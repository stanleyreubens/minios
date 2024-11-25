// include/thread/condition_variable.hpp
#pragma once
#include <mutex>
#include <condition_variable>

namespace os_sim {

class ConditionVariable {
public:
    ConditionVariable() = default;
    ~ConditionVariable() = default;

    // Non-template member functions (declared here, defined in cpp)
    void wait(std::unique_lock<std::mutex>& lock);
    void notify_one();
    void notify_all();
    
    // Template member function (must be defined in header)
    template<typename Predicate>
    void wait(std::unique_lock<std::mutex>& lock, Predicate pred) {
        cv_.wait(lock, std::move(pred));
    }

private:
    std::condition_variable cv_;
};

} // namespace os_sim