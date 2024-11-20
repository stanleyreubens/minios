// src/ipc/shared_memory.cpp
#include "ipc/shared_memory.hpp"
#include <cstring>
#include <stdexcept>

namespace os_sim {

SharedMemory::SharedMemory(const std::string& name, size_t size)
    : name_(name)
    , size_(size)
    , memory_ptr_(nullptr)
{
    if (!createSharedMemory()) {
        throw std::runtime_error("Failed to create shared memory");
    }
}

SharedMemory::~SharedMemory() {
    destroySharedMemory();
}

bool SharedMemory::write(const void* data, size_t size, size_t offset) {
    if (offset + size > size_) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    uint8_t* dest = static_cast<uint8_t*>(memory_ptr_) + offset;
    std::memcpy(dest, data, size);
    return true;
}

bool SharedMemory::read(void* buffer, size_t size, size_t offset) {
    if (offset + size > size_) {
        return false;
    }
    
    std::lock_guard<std::mutex> lock(mutex_);
    const uint8_t* src = static_cast<const uint8_t*>(memory_ptr_) + offset;
    std::memcpy(buffer, src, size);
    return true;
}

bool SharedMemory::createSharedMemory() {
    // Simple implementation using heap memory
    memory_ptr_ = new uint8_t[size_];
    return memory_ptr_ != nullptr;
}

void SharedMemory::destroySharedMemory() {
    if (memory_ptr_) {
        delete[] static_cast<uint8_t*>(memory_ptr_);
        memory_ptr_ = nullptr;
    }
}

} // namespace os_sim