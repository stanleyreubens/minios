
#pragma once
#include <cstdint>
#include <string>

namespace os_sim {

enum class IPCMessageType {
    NORMAL,
    PRIORITY,
    SYSTEM,
    ERROR
};


struct IPCMessage {
    IPCMessageType type;
    uint32_t sender_pid;
    uint32_t receiver_pid;
    std::string content;
    uint64_t timestamp;
};


enum class IPCError {
    SUCCESS,
    BUFFER_FULL,
    BUFFER_EMPTY,
    INVALID_SIZE,
    ACCESS_DENIED,
    NOT_FOUND
};

} 