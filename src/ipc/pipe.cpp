// src/ipc/pipe.cpp
#include "ipc/pipe.hpp"
#include <algorithm>
#include <cstring>

namespace os_sim {

Pipe::Pipe(size_t buffer_size)
    : buffer_(buffer_size)
    , read_pos_(0)
    , write_pos_(0)
{}

Pipe::~Pipe() = default;

IPCError Pipe::write(const void* data, size_t size) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // Wait until there's enough space
    not_full_.wait(lock, [this, size]() {
        return (write_pos_ + size) <= buffer_.size();
    });
    
    // Copy data to buffer
    const uint8_t* src = static_cast<const uint8_t*>(data);
    std::copy(src, src + size, buffer_.begin() + write_pos_);
    write_pos_ += size;
    
    not_empty_.notify_one();
    return IPCError::SUCCESS;
}

IPCError Pipe::read(void* buffer, size_t size) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // Wait until there's enough data
    not_empty_.wait(lock, [this, size]() {
        return (write_pos_ - read_pos_) >= size;
    });
    
    // Copy data from buffer
    uint8_t* dest = static_cast<uint8_t*>(buffer);
    std::copy(buffer_.begin() + read_pos_,
             buffer_.begin() + read_pos_ + size,
             dest);
    read_pos_ += size;
    
    // Reset positions if buffer is empty
    if (read_pos_ == write_pos_) {
        read_pos_ = write_pos_ = 0;
    }
    
    not_full_.notify_one();
    return IPCError::SUCCESS;
}

IPCError Pipe::tryWrite(const void* data, size_t size) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (write_pos_ + size > buffer_.size()) {
        return IPCError::BUFFER_FULL;
    }
    
    const uint8_t* src = static_cast<const uint8_t*>(data);
    std::copy(src, src + size, buffer_.begin() + write_pos_);
    write_pos_ += size;
    
    not_empty_.notify_one();
    return IPCError::SUCCESS;
}

IPCError Pipe::tryRead(void* buffer, size_t size) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (write_pos_ - read_pos_ < size) {
        return IPCError::BUFFER_EMPTY;
    }
    
    uint8_t* dest = static_cast<uint8_t*>(buffer);
    std::copy(buffer_.begin() + read_pos_,
             buffer_.begin() + read_pos_ + size,
             dest);
    read_pos_ += size;
    
    if (read_pos_ == write_pos_) {
        read_pos_ = write_pos_ = 0;
    }
    
    not_full_.notify_one();
    return IPCError::SUCCESS;
}

size_t Pipe::available() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return write_pos_ - read_pos_;
}

bool Pipe::isEmpty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return read_pos_ == write_pos_;
}

bool Pipe::isFull() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return write_pos_ == buffer_.size();
}

} // namespace os_sim