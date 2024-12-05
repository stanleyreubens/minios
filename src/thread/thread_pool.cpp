// src/thread/thread_pool.cpp
#include "thread/thread_pool.hpp"
#include <algorithm>
#include <stdexcept>

namespace os_sim {

ThreadPool::ThreadPool(size_t num_threads)
    : stop_(false)
    , paused_(false)
    , active_threads_(0)
{
    workers_.reserve(num_threads);

    for (size_t i = 0; i < num_threads; ++i) {
        workers_.emplace_back(&ThreadPool::workerFunction, this);
    }
}

ThreadPool::~ThreadPool() {
    shutdown();
}

void ThreadPool::shutdown() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        stop_ = true;
    }
    
    condition_.notify_all();
    pause_condition_.notify_all();
    
    for (std::thread& worker : workers_) {
        if (worker.joinable()) {
            worker.join();
        }
    }
}

void ThreadPool::pause() {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    paused_ = true;
}

void ThreadPool::resume() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex_);
        paused_ = false;
    }
    pause_condition_.notify_all();
}

size_t ThreadPool::getActiveThreadCount() const {
    return active_threads_;
}

size_t ThreadPool::getQueuedTaskCount() const {
    std::lock_guard<std::mutex> lock(queue_mutex_);
    return tasks_.size();
}

double ThreadPool::getAverageWaitTime() const {
    uint64_t completed = total_tasks_completed_.load();
    if (completed == 0) return 0.0;
    
    return static_cast<double>(total_wait_time_.load()) / completed;
}

void ThreadPool::workerFunction() {
    while (true) {
        std::function<void()> task;
        
        {
            std::unique_lock<std::mutex> lock(queue_mutex_);
            
            // Wait for work or shutdown signal
            condition_.wait(lock, [this] {
                return stop_ || !tasks_.empty();
            });
            
            // Check if we should stop
            if (stop_ && tasks_.empty()) {
                return;
            }
            
            // Check if pool is paused
            while (paused_ && !stop_) {
                pause_condition_.wait(lock);
            }
            
            // Get next task
            if (!tasks_.empty()) {
                task = std::move(tasks_.front());
                tasks_.pop();
                ++active_threads_;
            }
        }
        
        // Execute task
        if (task) {
            task();
            --active_threads_;
            ++total_tasks_completed_;
        }
    }
}

} // namespace os_sim