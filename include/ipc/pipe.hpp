// include/ipc/pipe.hpp
#pragma once
#include "ipc_common.hpp"
#include <vector>
#include <mutex>
#include <condition_variable>
#include <cstdint>

namespace os_sim {

class Pipe {
public:
    explicit Pipe(size_t buffer_size = 4096);
    ~Pipe();
    
    // I/O operations
    IPCError write(const void* data, size_t size);
    IPCError read(void* buffer, size_t size);
    
    // Non-blocking operations
    IPCError tryWrite(const void* data, size_t size);
    IPCError tryRead(void* buffer, size_t size);
    
    // Pipe status
    size_t available() const;
    bool isEmpty() const;
    bool isFull() const;
    
private:
    std::vector<uint8_t> buffer_;
    size_t read_pos_;
    size_t write_pos_;
    mutable std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

} // namespace os_sim