// src/ipc/message_queue.cpp
#include "ipc/message_queue.hpp"
#include <chrono>

namespace os_sim {

MessageQueue::MessageQueue(size_t max_size)
    : max_size_(max_size)
{}

IPCError MessageQueue::send(const IPCMessage& message) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // Wait until queue has space
    not_full_.wait(lock, [this]() { return messages_.size() < max_size_; });
    
    messages_.push(message);
    not_empty_.notify_one();
    return IPCError::SUCCESS;
}

IPCError MessageQueue::receive(IPCMessage& message) {
    std::unique_lock<std::mutex> lock(mutex_);
    
    // Wait until queue has messages
    not_empty_.wait(lock, [this]() { return !messages_.empty(); });
    
    message = messages_.front();
    messages_.pop();
    not_full_.notify_one();
    return IPCError::SUCCESS;
}

IPCError MessageQueue::trySend(const IPCMessage& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (messages_.size() >= max_size_) {
        return IPCError::BUFFER_FULL;
    }
    
    messages_.push(message);
    not_empty_.notify_one();
    return IPCError::SUCCESS;
}

IPCError MessageQueue::tryReceive(IPCMessage& message) {
    std::lock_guard<std::mutex> lock(mutex_);
    
    if (messages_.empty()) {
        return IPCError::BUFFER_EMPTY;
    }
    
    message = messages_.front();
    messages_.pop();
    not_full_.notify_one();
    return IPCError::SUCCESS;
}

size_t MessageQueue::size() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return messages_.size();
}

bool MessageQueue::empty() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return messages_.empty();
}

bool MessageQueue::full() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return messages_.size() >= max_size_;
}

} // namespace os_sim