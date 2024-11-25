// include/thread/semaphore.hpp
#pragma once
#include <mutex>
#include <condition_variable>
#include <chrono>

namespace os_sim {

class Semaphore {
public:
    explicit Semaphore(int count = 1);
    
    void acquire();
    bool tryAcquire();
    bool tryAcquireFor(const std::chrono::milliseconds& timeout);
    void release();
    
    int getCount() const;

private:
    mutable std::mutex mutex_;
    std::condition_variable cv_;
    int count_;
};

} // namespace os_sim