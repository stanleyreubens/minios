// include/process/process.hpp
#pragma once
#include "types.hpp"
#include <memory>
#include <mutex>
#include <condition_variable>
#include <vector>
#include <queue>

namespace os_sim {

class Process {
public:
    Process(ProcessID pid, std::string name, Priority priority = 0);
    ~Process();  // Remove the = default

    // Basic getters
    ProcessID getPID() const { return pid_; }
    ProcessState getState() const;
    Priority getPriority() const;
    const std::string& getName() const { return name_; }
    const ProcessStats& getStats() const { return stats_; }

    // State management
    void setState(ProcessState new_state);
    void setPriority(Priority new_priority);

    // Resource management
    ErrorCode requestResource(ResourceID resource_id);
    ErrorCode releaseResource(ResourceID resource_id);
    bool hasResource(ResourceID resource_id) const;
    std::vector<ResourceID> getAllocatedResources() const;

    // Process control
    void suspend();
    void resume();
    void terminate();

private:
    ProcessID pid_;
    std::string name_;
    ProcessState state_;
    Priority priority_;
    ProcessStats stats_;

    std::vector<ResourceID> allocated_resources_;
    std::queue<std::string> message_queue_;

    mutable std::mutex process_mutex_;
    std::condition_variable state_cv_;

    // Internal helper methods
    void updateStats();
    bool canTransitionTo(ProcessState new_state) const;
};

} // namespace os_sim