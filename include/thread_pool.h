#pragma once

#include <atomic>
#include <vector>
#include <queue>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>

namespace server {

class thread_pool
{
private:
    // need to keep track of threads so we can join them
    std::vector< std::thread > workers_;
    // the task queue
    std::queue< std::function<void()> > tasks_;
    
    // synchronization
    std::mutex queue_mutex_;
    std::condition_variable condition_;
    std::atomic_bool stop_;

public:
    explicit thread_pool(size_t);
    ~thread_pool();

    // this object shouldn't be copied
    thread_pool(const thread_pool&) = delete;
    thread_pool& operator =(const thread_pool&) = delete;

    template<typename F, typename... Args>
    auto enqueue(F&& f, Args&&... args) -> std::future<typename std::result_of<F(Args...)>::type>;
};

} // namespace server
