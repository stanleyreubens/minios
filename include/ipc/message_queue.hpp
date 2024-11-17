// include/ipc/message_queue.hpp
#pragma once
#include "ipc_common.hpp"
#include <queue>
#include <mutex>
#include <condition_variable>

namespace os_sim {

class MessageQueue {
public:
    explicit MessageQueue(size_t max_size = 100);
    ~MessageQueue() = default;
    
    // Message operations
    IPCError send(const IPCMessage& message);
    IPCError receive(IPCMessage& message);
    
    // Non-blocking operations
    IPCError trySend(const IPCMessage& message);
    IPCError tryReceive(IPCMessage& message);
    
    // Queue information
    size_t size() const;
    bool empty() const;
    bool full() const;
    
private:
    size_t max_size_;
    std::queue<IPCMessage> messages_;
    mutable std::mutex mutex_;
    std::condition_variable not_full_;
    std::condition_variable not_empty_;
};

} // namespace os_sim