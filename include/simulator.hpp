// include/simulator.hpp
#pragma once
#include "thread/thread_pool.hpp"
#include <string>
#include <functional>
#include <map>
#include <vector>
#include <memory>

namespace os_sim {

class Simulator {
public:
    static Simulator& getInstance();

    // Initialize the simulator
    void initialize();
    
    // Run the simulator
    void run();
    
    // Command processing
    void processCommand(const std::string& command);
    
    // Shutdown
    void shutdown();

private:
    bool inCalculatorMode_ = false; // Tracks if user is interacting with the calculator
    Simulator() = default;
    ~Simulator() = default;
    Simulator(const Simulator&) = delete;
    Simulator& operator=(const Simulator&) = delete;
    
    // Command handlers
    using CommandHandler = std::function<void(const std::vector<std::string>&)>;
    std::map<std::string, CommandHandler> command_handlers_;
    
    // System components
    std::unique_ptr<ThreadPool> thread_pool_;
    
    // Command processing helpers
    void setupCommandHandlers();
    std::vector<std::string> parseCommand(const std::string& command);
    void displayHelp();
    
    // Command implementations
    void handleCreateProcess(const std::vector<std::string>& args);
    void handleTerminateProcess(const std::vector<std::string>& args);
    void handleListProcesses();
    void handleProcessInfo(const std::vector<std::string>& args);
    void handleAllocateResource(const std::vector<std::string>& args);
    void handleReleaseResource(const std::vector<std::string>& args);
    void handleCheckDeadlock();
    void handleSystemStatus();
    void handleListResources();  
    void handleStartCalculator(const std::vector<std::string>& args);
    void handleSuspendProcess(const std::vector<std::string>& args);
    void handleResumeProcess(const std::vector<std::string>& args);
};

} // namespace os_sim
