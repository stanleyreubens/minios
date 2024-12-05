// src/thread/condition_variable.cpp
#include "thread/condition_variable.hpp"

namespace os_sim {

void ConditionVariable::wait(std::unique_lock<std::mutex>& lock) {
    cv_.wait(lock);
}

void ConditionVariable::notify_one() {
    cv_.notify_one();
}

void ConditionVariable::notify_all() {
    cv_.notify_all();
}

} // namespace os_sim