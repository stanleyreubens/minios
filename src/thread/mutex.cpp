// src/thread/mutex.cpp
#include "thread/mutex.hpp"
#include <thread>

namespace os_sim {

Mutex::Mutex(const std::string& name)
    : name_(name)
    , is_locked_(false)
{}

bool Mutex::lock() {
    mutex_.lock();
    is_locked_ = true;
    return true;
}

bool Mutex::tryLock() {
    bool result = mutex_.try_lock();
    if (result) {
        is_locked_ = true;
    }
    return result;
}

bool Mutex::tryLockFor(const std::chrono::milliseconds& timeout) {
    auto deadline = std::chrono::steady_clock::now() + timeout;
    
    while (std::chrono::steady_clock::now() < deadline) {
        if (tryLock()) {
            return true;
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    
    return false;
}

void Mutex::unlock() {
    is_locked_ = false;
    mutex_.unlock();
}

} // namespace os_sim