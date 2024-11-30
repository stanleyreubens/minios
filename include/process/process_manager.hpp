// include/process/process_manager.hpp
#pragma once
#include "process/process.hpp"
#include <unordered_map>
#include <memory>
#include <mutex>

namespace os_sim {

class ProcessManager {
public:
    static ProcessManager& getInstance();
    
    // Process lifecycle management
    std::shared_ptr<Process> createProcess(const std::string& name, Priority priority = 0);
    ErrorCode terminateProcess(ProcessID pid);
    
    // Process control
    ErrorCode suspendProcess(ProcessID pid);
    ErrorCode resumeProcess(ProcessID pid);
    
    // Process scheduling
    void scheduleProcesses();
    
    // Process queries
    std::shared_ptr<Process> getProcess(ProcessID pid);
    std::vector<ProcessID> listProcesses() const;
    
    // System statistics
    size_t getProcessCount() const;
    ProcessStats getSystemStats() const;

private:
    ProcessManager() = default;
    ~ProcessManager() = default;
    ProcessManager(const ProcessManager&) = delete;
    ProcessManager& operator=(const ProcessManager&) = delete;

    std::unordered_map<ProcessID, std::shared_ptr<Process>> processes_;
    ProcessID next_pid_{0};
    mutable std::mutex manager_mutex_;

    // Helper methods
    ProcessID generateNextPID();
    void cleanupTerminatedProcesses();
    void updateSystemStats();
};

} // namespace os_sim