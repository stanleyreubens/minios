// src/simulator.cpp
#include "simulator.hpp"
#include "process/process.hpp"
#include "process/process_manager.hpp"
#include "resource/resource_manager.hpp"
#include "thread/thread_pool.hpp"
#include <iostream>
#include <sstream>
#include <iomanip>
#include <chrono>

extern void parseAndCalculate(const std::string& input);
extern void calculatorFunction();

namespace os_sim {

Simulator& Simulator::getInstance() {
    static Simulator instance;
    return instance;
}

void Simulator::initialize() {
    // Initialize thread pool
    thread_pool_ = std::make_unique<ThreadPool>(4);
    
    // Setup command handlers
    setupCommandHandlers();
    
    std::cout << "Mini Operating System Simulator Initialized\n";
    std::cout << "Type 'help' for available commands\n";
}

void Simulator::run() {
    std::string command;
    
    while (true) {
        std::cout << "\nos_sim> ";
        std::getline(std::cin, command);
        
        if (command == "exit" || command == "quit") {
            break;
        }
        
        processCommand(command);
    }
    
    shutdown();
}

void Simulator::handleListProcesses() {
    auto& pm = ProcessManager::getInstance();
    auto processes = pm.listProcesses();
    
    if (processes.empty()) {
        std::cout << "No active processes\n";
        return;
    }
    
    std::cout << "\nActive processes:\n";
    std::cout << std::setw(5) << "PID" << " | "
              << std::setw(20) << "Name" << " | "
              << std::setw(10) << "Priority" << " | "
              << "State\n";
    std::cout << std::string(50, '-') << "\n";
    
    for (ProcessID pid : processes) {
        auto process = pm.getProcess(pid);
        if (process) {
            std::cout << std::setw(5) << pid << " | "
                      << std::setw(20) << process->getName() << " | "
                      << std::setw(10) << process->getPriority() << " | "
                      << toString(process->getState()) << "\n";
        }
    }
}

void Simulator::handleCheckDeadlock() {
    auto& rm = ResourceManager::getInstance();
    if (rm.detectDeadlock()) {
        std::cout << "Deadlock detected! Attempting resolution...\n";
        rm.resolveDeadlock();
        std::cout << "Deadlock resolution complete\n";
    } else {
        std::cout << "No deadlock detected\n";
    }
}

void Simulator::handleSystemStatus() {
    auto& pm = ProcessManager::getInstance();
    auto& rm = ResourceManager::getInstance();
    
    std::cout << "\nSystem Status:\n";
    std::cout << "==============\n\n";
    
    // Process Information
    std::cout << "Active Processes: " << pm.getProcessCount() << "\n";
    auto processes = pm.listProcesses();
    if (!processes.empty()) {
        std::cout << "\nProcess Details:\n";
        std::cout << std::setw(5) << "PID" << " | "
                  << std::setw(20) << "Name" << " | "
                  << std::setw(10) << "Priority" << " | "
                  << std::setw(15) << "State" << " | "
                  << "Resources\n";
        std::cout << std::string(70, '-') << "\n";
        
        for (ProcessID pid : processes) {
            auto process = pm.getProcess(pid);
            if (process) {
                std::cout << std::setw(5) << pid << " | "
                          << std::setw(20) << process->getName() << " | "
                          << std::setw(10) << process->getPriority() << " | "
                          << std::setw(15) << toString(process->getState()) << " | ";
                
                // Show resources held by this process
                auto resources = rm.getProcessResources(pid);
                for (auto res_id : resources) {
                    std::cout << res_id << " ";
                }
                std::cout << "\n";
            }
        }
    }
    
    // Resource Information
    std::cout << "\nResource Status:\n";
    std::cout << "Total Resources: " << rm.getResourceCount() << "\n";
    std::cout << "Allocated Resources: " << rm.getAllocatedResourceCount() << "\n";
    
    auto available_resources = rm.getAvailableResources();
    std::cout << "Available Resources: ";
    for (auto res_id : available_resources) {
        std::cout << res_id << " ";
    }
    std::cout << "\n";
    
    // Check for deadlocks
    if (rm.detectDeadlock()) {
        std::cout << "\nWARNING: Deadlock detected in the system!\n";
    }
    
    std::cout << "\n";
}

void Simulator::processCommand(const std::string& command) {
    // Check if the simulator is in calculator mode
    if (inCalculatorMode_) {
        // Directly forward the input to the calculator's parseAndCalculate function
        parseAndCalculate(command);
        return; // Exit after processing the input in calculator mode
    }

    // Existing command parsing logic for normal simulator mode
    auto args = parseCommand(command);
    if (args.empty()) return;

    std::string cmd = args[0];
    args.erase(args.begin());
    
    auto it = command_handlers_.find(cmd);
    if (it != command_handlers_.end()) {
        try {
            it->second(args);
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
        }
    } else {
        std::cout << "Unknown command. Type 'help' for available commands.\n";
    }
}

void Simulator::shutdown() {
    std::cout << "Shutting down simulator...\n";
    
    if (thread_pool_) {
        thread_pool_->shutdown();
    }
    
    auto& pm = ProcessManager::getInstance();
    for (ProcessID pid : pm.listProcesses()) {
        pm.terminateProcess(pid);
    }
    
    std::cout << "Simulator shutdown complete.\n";
}

std::vector<std::string> Simulator::parseCommand(const std::string& command) {
    std::vector<std::string> args;
    std::istringstream iss(command);
    std::string arg;
    
    while (iss >> arg) {
        args.push_back(arg);
    }
    
    return args;
}

void Simulator::setupCommandHandlers() {
    command_handlers_["help"] = [this](const auto& /*args*/) { displayHelp(); };
    command_handlers_["create"] = [this](const auto& args) { handleCreateProcess(args); };
    command_handlers_["terminate"] = [this](const auto& args) { handleTerminateProcess(args); };
    command_handlers_["list"] = [this](const auto& /*args*/) { handleListProcesses(); };
    command_handlers_["info"] = [this](const auto& args) { handleProcessInfo(args); };
    command_handlers_["allocate"] = [this](const auto& args) { handleAllocateResource(args); };
    command_handlers_["release"] = [this](const auto& args) { handleReleaseResource(args); };
    command_handlers_["deadlock"] = [this](const auto& /*args*/) { handleCheckDeadlock(); };
    command_handlers_["status"] = [this](const auto& /*args*/) { handleSystemStatus(); };
    command_handlers_["resources"] = [this](const auto& /*args*/) { handleListResources(); };
    command_handlers_["suspend"] = [this](const auto& args) { handleSuspendProcess(args); };
    command_handlers_["resume"] = [this](const auto& args) { handleResumeProcess(args); };
    command_handlers_["calculator"] = [this](const auto& args) { handleStartCalculator(args); };
}

void Simulator::displayHelp() {
    std::cout << "\nAvailable commands:\n";
    std::cout << "  help                    - Display this help message\n";
    std::cout << "  create <name> [priority] - Create a new process\n";
    std::cout << "  terminate <pid>         - Terminate a process\n";
    std::cout << "  list                    - List all processes\n";
    std::cout << "  info <pid>              - Display process information\n";
    std::cout << "  allocate <pid> <res_id> - Allocate a resource to a process\n";
    std::cout << "  release <pid> <res_id>  - Release a resource from a process\n";
    std::cout << "  deadlock                - Check for deadlocks\n";
    std::cout << "  status                  - Display system status\n";
    std::cout << "  resources               - List resources\n";
    std::cout << "  calculator              - Start the calculator process\n"; 
    std::cout << "  suspend <pid>           - Suspend a process\n";
    std::cout << "  resume <pid>            - Resume a suspended process\n";
    std::cout << "  exit                    - Exit the simulator\n";
}

void Simulator::handleCreateProcess(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: create <name> [priority]\n";
        return;
    }
    
    std::string name = args[0];
    Priority priority = args.size() > 1 ? std::stoi(args[1]) : 0;
    
    auto& pm = ProcessManager::getInstance();
    auto process = pm.createProcess(name, priority);
    
    std::cout << "Created process '" << name << "' with PID " 
              << process->getPID() << " and priority " << priority << "\n";
}

void Simulator::handleTerminateProcess(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: terminate <pid>\n";
        return;
    }
    
    ProcessID pid = std::stoi(args[0]);
    auto& pm = ProcessManager::getInstance();
    
    if (pm.terminateProcess(pid) == ErrorCode::SUCCESS) {
        std::cout << "Process " << pid << " terminated\n";
    } else {
        std::cout << "Failed to terminate process " << pid << "\n";
    }
}

void Simulator::handleProcessInfo(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: info <pid>\n";
        return;
    }
    
    ProcessID pid = std::stoi(args[0]);
    auto& pm = ProcessManager::getInstance();
    auto process = pm.getProcess(pid);
    
    if (!process) {
        std::cout << "Process " << pid << " not found\n";
        return;
    }
    
    const auto& stats = process->getStats();
    std::cout << "\nProcess Information:\n";
    std::cout << "PID: " << pid << "\n";
    std::cout << "Name: " << process->getName() << "\n";
    std::cout << "Priority: " << process->getPriority() << "\n";
    std::cout << "State: " << toString(process->getState()) << "\n";
    std::cout << "CPU Time: " << stats.cpu_time << "ms\n";
    std::cout << "Memory Used: " << stats.memory_used << " bytes\n";
    std::cout << "I/O Operations: " << stats.io_operations << "\n";
    std::cout << "Context Switches: " << stats.context_switches << "\n";
}

void Simulator::handleListResources() {
    std::cout << "Fetching resource manager..." << std::endl;
    auto& rm = ResourceManager::getInstance();
    
    std::cout << "Getting available resources..." << std::endl;
    auto available = rm.getAvailableResources();
    
    std::cout << "\nAvailable Resources:\n";
    std::cout << "ID | Type | Status\n";
    std::cout << std::string(30, '-') << "\n";
    
    std::cout << "Processing " << available.size() << " available resources..." << std::endl;
    for (auto res_id : available) {
        auto type = rm.getResourceType(res_id);
        std::cout << std::setw(2) << res_id << " | "
                  << std::setw(6) << toString(type) << " | "
                  << "Available\n";
    }
    
    std::cout << "Getting allocations..." << std::endl;
    auto allocations = rm.getAllocations();
    for (const auto& [pid, rid] : allocations) {
        auto type = rm.getResourceType(rid);
        std::cout << std::setw(2) << rid << " | "
                  << std::setw(6) << toString(type) << " | "
                  << "Allocated to Process " << pid << "\n";
    }
    
    std::cout << "\nResource listing complete." << std::endl;
}


void Simulator::handleAllocateResource(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: allocate <pid> <resource_id>\n";
        return;
    }
    
    ProcessID pid = std::stoi(args[0]);
    ResourceID rid = std::stoi(args[1]);
    
    auto& rm = ResourceManager::getInstance();
    if (rm.allocateResource(pid, rid) == ErrorCode::SUCCESS) {
        std::cout << "Resource " << rid << " allocated to process " << pid << "\n";
    } else {
        std::cout << "Failed to allocate resource " << rid << " to process " << pid << "\n";
    }
}

void Simulator::handleStartCalculator(const std::vector<std::string>& /*args*/) {
    auto& pm = ProcessManager::getInstance();
    auto process = pm.createProcess("Calculator", 5);
    std::cout << "Calculator process created with PID " << process->getPID() << "\n";

    inCalculatorMode_ = true; // Enter calculator mode

    // Run calculator function in the current thread for simplicity
    calculatorFunction();

    inCalculatorMode_ = false; // Exit calculator mode
}

void Simulator::handleReleaseResource(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        std::cout << "Usage: release <pid> <resource_id>\n";
        return;
    }
    
    ProcessID pid = std::stoi(args[0]);
    ResourceID rid = std::stoi(args[1]);
    
    auto& rm = ResourceManager::getInstance();
    if (rm.releaseResource(pid, rid) == ErrorCode::SUCCESS) {
        std::cout << "Resource " << rid << " released from process " << pid << "\n";
    } else {
        std::cout << "Failed to release resource " << rid << " from process " << pid << "\n";
    }
}

void Simulator::handleSuspendProcess(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: suspend <pid>\n";
        return;
    }
    
    ProcessID pid = std::stoi(args[0]);
    auto& pm = ProcessManager::getInstance();
    
    if (pm.suspendProcess(pid) == ErrorCode::SUCCESS) {
        std::cout << "Process " << pid << " suspended\n";
    } else {
        std::cout << "Failed to suspend process " << pid << "\n";
    }
}

void Simulator::handleResumeProcess(const std::vector<std::string>& args) {
    if (args.empty()) {
        std::cout << "Usage: resume <pid>\n";
        return;
    }
    
    ProcessID pid = std::stoi(args[0]);
    auto& pm = ProcessManager::getInstance();
    
    if (pm.resumeProcess(pid) == ErrorCode::SUCCESS) {
        std::cout << "Process " << pid << " resumed\n";
    } else {
        std::cout << "Failed to resume process " << pid << "\n";
    }
}

} // namespace os_sim