// include/thread/mutex.hpp
#pragma once
#include <mutex>
#include <string>
#include <chrono>

namespace os_sim {

class Mutex {
public:
    explicit Mutex(const std::string& name = "");
    ~Mutex() = default;

    bool lock();
    bool tryLock();
    bool tryLockFor(const std::chrono::milliseconds& timeout);
    void unlock();

    // Inline getters
    const std::string& getName() const { return name_; }
    bool isLocked() const { return is_locked_; }

private:
    std::string name_;
    std::mutex mutex_;
    bool is_locked_;
};

} // namespace os_sim