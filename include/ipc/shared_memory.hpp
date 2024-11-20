// include/ipc/shared_memory.hpp
#pragma once
#include <string>
#include <mutex>
#include <cstddef>

namespace os_sim {

class SharedMemory {
public:
    SharedMemory(const std::string& name, size_t size);
    ~SharedMemory();

    // Memory operations
    bool write(const void* data, size_t size, size_t offset = 0);
    bool read(void* buffer, size_t size, size_t offset = 0);
    
    // Memory information
    void* getPointer() { return memory_ptr_; }
    const void* getPointer() const { return memory_ptr_; }
    size_t getSize() const { return size_; }
    const std::string& getName() const { return name_; }

private:
    std::string name_;
    size_t size_;
    void* memory_ptr_;
    std::mutex mutex_;

    bool createSharedMemory();
    void destroySharedMemory();
};

} // namespace os_sim