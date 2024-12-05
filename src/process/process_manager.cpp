// src/process/process_manager.cpp
#include "process/process_manager.hpp"
#include <algorithm>

namespace os_sim {

ProcessManager& ProcessManager::getInstance() {
    static ProcessManager instance;
    return instance;
}

std::shared_ptr<Process> ProcessManager::createProcess(
    const std::string& name, Priority priority) {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    
    ProcessID pid = generateNextPID();
    auto process = std::make_shared<Process>(pid, name, priority);
    processes_[pid] = process;
    
    // Schedule the new process
    process->setState(ProcessState::READY);
    return process;
}

ErrorCode ProcessManager::terminateProcess(ProcessID pid) {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    
    auto it = processes_.find(pid);
    if (it == processes_.end()) {
        return ErrorCode::PROCESS_NOT_FOUND;
    }
    
    auto process = it->second;
    process->setState(ProcessState::TERMINATED);
    processes_.erase(it);
    return ErrorCode::SUCCESS;
}

ErrorCode ProcessManager::suspendProcess(ProcessID pid) {
    auto process = getProcess(pid);
    if (!process) {
        return ErrorCode::PROCESS_NOT_FOUND;
    }
    
    process->suspend();
    return ErrorCode::SUCCESS;
}

ErrorCode ProcessManager::resumeProcess(ProcessID pid) {
    auto process = getProcess(pid);
    if (!process) {
        return ErrorCode::PROCESS_NOT_FOUND;
    }
    
    process->resume();
    return ErrorCode::SUCCESS;
}

void ProcessManager::scheduleProcesses() {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    
    // First, handle any running process
    for (const auto& [pid, process] : processes_) {
        if (process->getState() == ProcessState::RUNNING) {
            process->setState(ProcessState::READY);
            break;  // Only one process should be running
        }
    }
    
    // Get all ready processes
    std::vector<std::shared_ptr<Process>> ready_processes;
    for (const auto& [pid, process] : processes_) {
        if (process->getState() == ProcessState::READY) {
            ready_processes.push_back(process);
        }
    }
    
    if (ready_processes.empty()) {
        return;
    }
    
    // Sort by priority
    std::sort(ready_processes.begin(), ready_processes.end(),
        [](const auto& a, const auto& b) {
            return a->getPriority() > b->getPriority();
        });
    
    // Set highest priority process to RUNNING
    ready_processes[0]->setState(ProcessState::RUNNING);
}

std::shared_ptr<Process> ProcessManager::getProcess(ProcessID pid) {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    auto it = processes_.find(pid);
    return (it != processes_.end()) ? it->second : nullptr;
}

std::vector<ProcessID> ProcessManager::listProcesses() const {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    std::vector<ProcessID> pids;
    pids.reserve(processes_.size());
    
    for (const auto& [pid, _] : processes_) {
        pids.push_back(pid);
    }
    
    return pids;
}

size_t ProcessManager::getProcessCount() const {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    return processes_.size();
}

ProcessStats ProcessManager::getSystemStats() const {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    ProcessStats system_stats;
    
    for (const auto& [_, process] : processes_) {
        const auto& stats = process->getStats();
        system_stats.cpu_time += stats.cpu_time;
        system_stats.memory_used += stats.memory_used;
        system_stats.io_operations += stats.io_operations;
        system_stats.context_switches += stats.context_switches;
    }
    
    return system_stats;
}

ProcessID ProcessManager::generateNextPID() {
    return next_pid_++;
}

void ProcessManager::cleanupTerminatedProcesses() {
    std::lock_guard<std::mutex> lock(manager_mutex_);
    
    for (auto it = processes_.begin(); it != processes_.end();) {
        if (it->second->getState() == ProcessState::TERMINATED) {
            it = processes_.erase(it);
        } else {
            ++it;
        }
    }
}

void ProcessManager::updateSystemStats() {
    // This could be expanded to track additional system-wide metrics
}

} // namespace os_sim