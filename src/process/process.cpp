// src/process/process.cpp
#include "process/process.hpp"
#include "resource/resource_manager.hpp"
#include <algorithm>
#include <chrono>

namespace os_sim {

Process::Process(ProcessID pid, std::string name, Priority priority)
    : pid_(pid)
    , name_(std::move(name))
    , state_(ProcessState::NEW)
    , priority_(priority)
{
    updateStats();
}

Process::~Process() {
    // Release all resources
    auto resources = getAllocatedResources();
    for (const auto& resource_id : resources) {
        releaseResource(resource_id);
    }
}

ProcessState Process::getState() const {
    std::lock_guard<std::mutex> lock(process_mutex_);
    return state_;
}

Priority Process::getPriority() const {
    std::lock_guard<std::mutex> lock(process_mutex_);
    return priority_;
}

void Process::setState(ProcessState new_state) {
    std::lock_guard<std::mutex> lock(process_mutex_);
    
    if (!canTransitionTo(new_state)) {
        throw std::runtime_error("Invalid state transition");
    }
    
    state_ = new_state;
    updateStats();
    state_cv_.notify_all();
}

void Process::setPriority(Priority new_priority) {
    std::lock_guard<std::mutex> lock(process_mutex_);
    priority_ = new_priority;
}

ErrorCode Process::requestResource(ResourceID resource_id) {
    std::lock_guard<std::mutex> lock(process_mutex_);
    
    auto& rm = ResourceManager::getInstance();
    auto result = rm.allocateResource(pid_, resource_id);
    
    if (result == ErrorCode::SUCCESS) {
        allocated_resources_.push_back(resource_id);
        updateStats();
    }
    
    return result;
}

ErrorCode Process::releaseResource(ResourceID resource_id) {
    std::lock_guard<std::mutex> lock(process_mutex_);
    
    if (!hasResource(resource_id)) {
        return ErrorCode::RESOURCE_NOT_FOUND;
    }
    
    auto& rm = ResourceManager::getInstance();
    auto result = rm.releaseResource(pid_, resource_id);
    
    if (result == ErrorCode::SUCCESS) {
        auto it = std::find(allocated_resources_.begin(), 
                           allocated_resources_.end(), 
                           resource_id);
        if (it != allocated_resources_.end()) {
            allocated_resources_.erase(it);
        }
        updateStats();
    }
    
    return result;
}

bool Process::hasResource(ResourceID resource_id) const {
    std::lock_guard<std::mutex> lock(process_mutex_);
    return std::find(allocated_resources_.begin(), 
                    allocated_resources_.end(), 
                    resource_id) != allocated_resources_.end();
}

std::vector<ResourceID> Process::getAllocatedResources() const {
    std::lock_guard<std::mutex> lock(process_mutex_);
    return allocated_resources_;
}

void Process::suspend() {
    setState(ProcessState::WAITING);
}

void Process::resume() {
    setState(ProcessState::READY);
}

void Process::terminate() {
    setState(ProcessState::TERMINATED);
}

void Process::updateStats() {
    // Update process statistics
    stats_.context_switches++;
    
    // Update CPU time if process was running
    if (state_ == ProcessState::RUNNING) {
        static auto last_update = std::chrono::steady_clock::now();
        auto now = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>
                       (now - last_update);
        stats_.cpu_time += duration.count();
        last_update = now;
    }
    
    // Update memory usage based on allocated resources
    stats_.memory_used = allocated_resources_.size() * 1024; // Simple estimation
}

bool Process::canTransitionTo(ProcessState new_state) const {
    switch (state_) {
        case ProcessState::NEW:
            return new_state == ProcessState::READY;
            
        case ProcessState::READY:
            return new_state == ProcessState::RUNNING || 
                   new_state == ProcessState::TERMINATED;
            
        case ProcessState::RUNNING:
            return new_state == ProcessState::READY || 
                   new_state == ProcessState::WAITING || 
                   new_state == ProcessState::TERMINATED;
            
        case ProcessState::WAITING:
            return new_state == ProcessState::READY || 
                   new_state == ProcessState::TERMINATED;
            
        case ProcessState::TERMINATED:
            return false; // Cannot transition from terminated state
            
        default:
            return false;
    }
}

} // namespace os_sim