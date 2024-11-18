// include/types.hpp
#pragma once
#include <cstdint>
#include <string>

namespace os_sim {

using ProcessID = int32_t;
using ResourceID = int32_t;
using Priority = int32_t;

// Common error codes
enum class ErrorCode {
    SUCCESS = 0,
    RESOURCE_NOT_AVAILABLE,
    RESOURCE_NOT_FOUND,
    PROCESS_NOT_FOUND,
    INVALID_STATE,
    DEADLOCK_DETECTED,
    OPERATION_FAILED
};

// Process states following typical OS process lifecycle
enum class ProcessState {
    NEW,        // Process is being created
    READY,      // Process is ready to run
    RUNNING,    // Process is currently running
    WAITING,    // Process is waiting for some resource
    TERMINATED  // Process has finished execution
};

// Resource types that can be managed by the system
enum class ResourceType {
    CPU,
    MEMORY,
    FILE,
    NETWORK,
    GENERIC
};

// Process statistics structure
struct ProcessStats {
    uint64_t cpu_time{0};         // CPU time used
    uint64_t memory_used{0};      // Memory currently allocated
    uint64_t io_operations{0};    // Number of I/O operations
    uint64_t context_switches{0}; // Number of context switches
};

// String conversion functions
inline const char* toString(ProcessState state) {
    switch (state) {
        case ProcessState::NEW: return "NEW";
        case ProcessState::READY: return "READY";
        case ProcessState::RUNNING: return "RUNNING";
        case ProcessState::WAITING: return "WAITING";
        case ProcessState::TERMINATED: return "TERMINATED";
        default: return "UNKNOWN";
    }
}

inline const char* toString(ResourceType type) {
    switch (type) {
        case ResourceType::CPU: return "CPU";
        case ResourceType::MEMORY: return "MEM";
        case ResourceType::FILE: return "FILE";
        case ResourceType::NETWORK: return "NET";
        case ResourceType::GENERIC: return "GEN";
        default: return "UNK";
    }
}

inline const char* toString(ErrorCode error) {
    switch (error) {
        case ErrorCode::SUCCESS: return "SUCCESS";
        case ErrorCode::RESOURCE_NOT_AVAILABLE: return "RESOURCE_NOT_AVAILABLE";
        case ErrorCode::RESOURCE_NOT_FOUND: return "RESOURCE_NOT_FOUND";
        case ErrorCode::PROCESS_NOT_FOUND: return "PROCESS_NOT_FOUND";
        case ErrorCode::INVALID_STATE: return "INVALID_STATE";
        case ErrorCode::DEADLOCK_DETECTED: return "DEADLOCK_DETECTED";
        case ErrorCode::OPERATION_FAILED: return "OPERATION_FAILED";
        default: return "UNKNOWN_ERROR";
    }
}

} // namespace os_sim