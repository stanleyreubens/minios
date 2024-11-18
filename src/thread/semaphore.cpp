// src/thread/semaphore.cpp
#include "thread/semaphore.hpp"

namespace os_sim {

Semaphore::Semaphore(int count)
    : count_(count)
{}

void Semaphore::acquire() {
    std::unique_lock<std::mutex> lock(mutex_);
    cv_.wait(lock, [this]() { return count_ > 0; });
    --count_;
}

bool Semaphore::tryAcquire() {
    std::lock_guard<std::mutex> lock(mutex_);
    if (count_ > 0) {
        --count_;
        return true;
    }
    return false;
}

bool Semaphore::tryAcquireFor(const std::chrono::milliseconds& timeout) {
    std::unique_lock<std::mutex> lock(mutex_);
    if (cv_.wait_for(lock, timeout, [this]() { return count_ > 0; })) {
        --count_;
        return true;
    }
    return false;
}

void Semaphore::release() {
    std::lock_guard<std::mutex> lock(mutex_);
    ++count_;
    cv_.notify_one();
}

int Semaphore::getCount() const {
    std::lock_guard<std::mutex> lock(mutex_);
    return count_;
}

} // namespace os_sim