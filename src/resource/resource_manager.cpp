// src/resource/resource_manager.cpp
#include "resource/resource_manager.hpp"
#include "process/process.hpp"
#include <algorithm>
#include <iostream>
#include <thread>

namespace os_sim {

ResourceManager& ResourceManager::getInstance() {
    std::cout << "Getting ResourceManager instance..." << std::endl;
    static ResourceManager instance;
    std::cout << "ResourceManager instance created." << std::endl;
    return instance;
}

ResourceManager::ResourceManager() {
    std::cout << "Creating ResourceManager..." << std::endl;
    try {
        // Create resources without locking in constructor
        resources_[0] = ResourceType::CPU;
        resources_[1] = ResourceType::MEMORY;
        resources_[2] = ResourceType::FILE;
        resources_[3] = ResourceType::NETWORK;
        resources_[4] = ResourceType::GENERIC;
        next_resource_id_ = 5;  // Set next ID after initialization
        
        std::cout << "ResourceManager: Created 5 default resources" << std::endl;
    } catch (const std::exception& e) {
        std::cout << "Exception during initialization: " << e.what() << std::endl;
        throw;
    }
    std::cout << "ResourceManager constructor complete." << std::endl;
}

void ResourceManager::initializeDefaultResources() {
    std::cout << "Entering initializeDefaultResources..." << std::endl;
    try {
        std::cout << "Taking mutex lock..." << std::endl;
        std::lock_guard<std::mutex> lock(resource_mutex_);
        std::cout << "Mutex lock acquired." << std::endl;

        // Create default resources one by one with debug output
        std::cout << "Creating CPU resource..." << std::endl;
        createResource(ResourceType::CPU);
        std::cout << "CPU resource created." << std::endl;

        std::cout << "Creating MEMORY resource..." << std::endl;
        createResource(ResourceType::MEMORY);
        std::cout << "MEMORY resource created." << std::endl;

        std::cout << "Creating FILE resource..." << std::endl;
        createResource(ResourceType::FILE);
        std::cout << "FILE resource created." << std::endl;

        std::cout << "Creating NETWORK resource..." << std::endl;
        createResource(ResourceType::NETWORK);
        std::cout << "NETWORK resource created." << std::endl;

        std::cout << "Creating GENERIC resource..." << std::endl;
        createResource(ResourceType::GENERIC);
        std::cout << "GENERIC resource created." << std::endl;

    } catch (const std::exception& e) {
        std::cout << "Exception in initializeDefaultResources: " << e.what() << std::endl;
        throw;
    } catch (...) {
        std::cout << "Unknown exception in initializeDefaultResources" << std::endl;
        throw;
    }
    std::cout << "Default resources initialized successfully." << std::endl;
}

ResourceID ResourceManager::createResource(ResourceType type) {
    std::cout << "Creating resource of type " << toString(type) << std::endl;
    try {
        std::cout << "Creating with ID " << next_resource_id_ << std::endl;
        ResourceID id = next_resource_id_++;
        resources_[id] = type;
        std::cout << "Resource created successfully." << std::endl;
        return id;
    } catch (const std::exception& e) {
        std::cout << "Exception in createResource: " << e.what() << std::endl;
        throw;
    }
}

ErrorCode ResourceManager::destroyResource(ResourceID resource_id) {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    
    if (resources_.find(resource_id) == resources_.end()) {
        return ErrorCode::RESOURCE_NOT_AVAILABLE;
    }
    
    if (allocations_.find(resource_id) != allocations_.end()) {
        return ErrorCode::RESOURCE_NOT_AVAILABLE;  // Resource is in use
    }
    
    resources_.erase(resource_id);
    return ErrorCode::SUCCESS;
}

ErrorCode ResourceManager::allocateResource(ProcessID pid, ResourceID resource_id) {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    
    // Check if resource exists
    if (resources_.find(resource_id) == resources_.end()) {
        return ErrorCode::RESOURCE_NOT_AVAILABLE;
    }
    
    // Check if resource is already allocated
    if (allocations_.find(resource_id) != allocations_.end()) {
        return ErrorCode::RESOURCE_NOT_AVAILABLE;
    }
    
    // Allocate the resource
    allocations_[resource_id] = pid;
    process_resources_[pid].push_back(resource_id);
    
    return ErrorCode::SUCCESS;
}

ErrorCode ResourceManager::releaseResource(ProcessID pid, ResourceID resource_id) {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    
    // Check if resource is allocated to this process
    auto it = allocations_.find(resource_id);
    if (it == allocations_.end() || it->second != pid) {
        return ErrorCode::RESOURCE_NOT_AVAILABLE;
    }
    
    // Release the resource
    allocations_.erase(resource_id);
    
    // Remove from process resources
    auto& resources = process_resources_[pid];
    resources.erase(
        std::remove(resources.begin(), resources.end(), resource_id),
        resources.end()
    );
    
    return ErrorCode::SUCCESS;
}

bool ResourceManager::isResourceAvailable(ResourceID resource_id) const {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    return resources_.find(resource_id) != resources_.end() &&
           allocations_.find(resource_id) == allocations_.end();
}

std::vector<ResourceID> ResourceManager::getAvailableResources() const {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    std::vector<ResourceID> available;
    
    for (const auto& [id, _] : resources_) {
        if (allocations_.find(id) == allocations_.end()) {
            available.push_back(id);
        }
    }
    
    return available;
}

std::vector<ResourceID> ResourceManager::getProcessResources(ProcessID pid) const {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    auto it = process_resources_.find(pid);
    return (it != process_resources_.end()) ? it->second : std::vector<ResourceID>{};
}

bool ResourceManager::detectDeadlock() {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    std::vector<bool> visited(next_resource_id_, false);
    std::vector<bool> rec_stack(next_resource_id_, false);
    
    for (const auto& [pid, _] : process_resources_) {
        if (hasCycle(visited, rec_stack, pid)) {
            return true;
        }
    }
    
    return false;
}

bool ResourceManager::hasCycle(std::vector<bool>& visited, 
                             std::vector<bool>& rec_stack, 
                             ProcessID pid) {
    if (!visited[pid]) {
        visited[pid] = true;
        rec_stack[pid] = true;
        
        // Check resources this process is waiting for
        const auto& resources = process_resources_[pid];
        for (ResourceID res_id : resources) {
            auto alloc_it = allocations_.find(res_id);
            if (alloc_it != allocations_.end()) {
                ProcessID next_pid = alloc_it->second;
                if (!visited[next_pid] && hasCycle(visited, rec_stack, next_pid)) {
                    return true;
                } else if (rec_stack[next_pid]) {
                    return true;
                }
            }
        }
    }
    
    rec_stack[pid] = false;
    return false;
}

void ResourceManager::resolveDeadlock() {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    
    if (process_resources_.empty()) {
        return;
    }
    
    // Find process with highest PID
    ProcessID highest_pid = 0;
    for (const auto& [pid, _] : process_resources_) {
        highest_pid = std::max(highest_pid, pid);
    }
    
    // Release all its resources
    auto resources = process_resources_[highest_pid];
    for (ResourceID res_id : resources) {
        releaseResource(highest_pid, res_id);
    }
}

size_t ResourceManager::getResourceCount() const {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    return resources_.size();
}

size_t ResourceManager::getAllocatedResourceCount() const {
    std::lock_guard<std::mutex> lock(resource_mutex_);
    return allocations_.size();
}

} // namespace os_sim


//This was committed by Sirak.