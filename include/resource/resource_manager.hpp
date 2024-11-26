// include/resource/resource_manager.hpp
#pragma once
#include "types.hpp"
#include <unordered_map>
#include <vector>
#include <mutex>
#include <memory>
#include <iostream>  

namespace os_sim {

class ResourceManager {
public:
    static ResourceManager& getInstance();

    // Resource creation and management
    ResourceID createResource(ResourceType type = ResourceType::GENERIC);
    ErrorCode destroyResource(ResourceID resource_id);
    
    // Resource allocation
    ErrorCode allocateResource(ProcessID pid, ResourceID resource_id);
    ErrorCode releaseResource(ProcessID pid, ResourceID resource_id);
    bool isResourceAvailable(ResourceID resource_id) const;

    // Resource information
    std::vector<ResourceID> getAvailableResources() const;
    std::vector<ResourceID> getProcessResources(ProcessID pid) const;
    
    // Deadlock management
    bool detectDeadlock();
    void resolveDeadlock();

    // System statistics
    size_t getResourceCount() const;
    size_t getAllocatedResourceCount() const;
    
    // Resource type information
    ResourceType getResourceType(ResourceID id) const {
        std::lock_guard<std::mutex> lock(resource_mutex_);
        auto it = resources_.find(id);
        return it != resources_.end() ? it->second : ResourceType::GENERIC;
    }

    // Allocation information
    std::vector<std::pair<ProcessID, ResourceID>> getAllocations() const {
        std::lock_guard<std::mutex> lock(resource_mutex_);
        std::vector<std::pair<ProcessID, ResourceID>> result;
        for (const auto& [rid, pid] : allocations_) {
            result.emplace_back(pid, rid);
        }
        return result;
    }
    
private:
    // Private constructor for singleton
    ResourceManager();
    ~ResourceManager() = default;
    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    // Initialize default resources
    void initializeDefaultResources();

    // Member variables
    std::unordered_map<ResourceID, ResourceType> resources_;
    std::unordered_map<ResourceID, ProcessID> allocations_;
    std::unordered_map<ProcessID, std::vector<ResourceID>> process_resources_;
    
    mutable std::mutex resource_mutex_;
    ResourceID next_resource_id_{0};

    // Helper methods
    bool hasCycle(std::vector<bool>& visited, std::vector<bool>& rec_stack, ProcessID pid);
};

} // namespace os_sim

//This was committed by Sirak.