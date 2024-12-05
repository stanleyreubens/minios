// include/thread/thread_pool.hpp
#pragma once
#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <functional>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <chrono>

namespace os_sim {

class ThreadPool {
public:
    explicit ThreadPool(size_t num_threads);
    ~ThreadPool();

    // Task submission
    template<class F, class... Args>
    auto enqueue(F&& f, Args&&... args) 
        -> std::future<typename std::result_of<F(Args...)>::type>;

    // Pool management
    void shutdown();
    void pause();
    void resume();

    // Statistics
    size_t getActiveThreadCount() const;
    size_t getQueuedTaskCount() const;
    double getAverageWaitTime() const;

private:
    std::vector<std::thread> workers_;
    std::queue<std::function<void()>> tasks_;

    mutable std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::condition_variable pause_condition_;

    bool stop_;
    bool paused_;
    size_t active_threads_;

    // Performance metrics
    std::atomic<uint64_t> total_tasks_completed_{0};
    std::atomic<uint64_t> total_wait_time_{0};

    void workerFunction();
};

// Template implementation must be in header
template<class F, class... Args>
auto ThreadPool::enqueue(F&& f, Args&&... args) 
    -> std::future<typename std::result_of<F(Args...)>::type>
{
    using return_type = typename std::result_of<F(Args...)>::type;
    
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        std::bind(std::forward<F>(f), std::forward<Args>(args)...)
    );
    
    std::future<return_type> res = task->get_future();
    
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        
        if (stop_) {
            throw std::runtime_error("Cannot enqueue on stopped ThreadPool");
        }
        
        auto enqueue_time = std::chrono::steady_clock::now();
        
        tasks_.emplace([task, this, enqueue_time]() {
            auto start_time = std::chrono::steady_clock::now();
            auto wait_time = std::chrono::duration_cast<std::chrono::milliseconds>(
                start_time - enqueue_time).count();
            total_wait_time_ += wait_time;
            (*task)();
        });
    }
    
    condition_.notify_one();
    return res;
}

} // namespace os_sim