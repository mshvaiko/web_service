#include "thread_pool.h"

#include <iostream>
#include <memory>

namespace server {

thread_pool::thread_pool(size_t pool_size) : stop_(false) {
  for (size_t i = 0; i < pool_size; ++i)
    workers_.emplace_back([this] {
      for (;;) {
        std::function<void()> task;
        {
          std::unique_lock<std::mutex> lock(this->queue_mutex_);
          this->condition_.wait(
              lock, [this] { return this->stop_ || !this->tasks_.empty(); });

          if (this->stop_ && this->tasks_.empty())
            return;

          task = std::move(this->tasks_.front());
          this->tasks_.pop();
        }
        task();
      }
    });
}

void thread_pool::enqueue(std::function<void()> task) {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);

    // don't allow enqueueing after stopping the pool
    if (stop_)
      throw std::runtime_error("enqueue on stopped ThreadPool");

    tasks_.emplace(task);
  }
  condition_.notify_one();
}

thread_pool::~thread_pool() {
  {
    std::unique_lock<std::mutex> lock(queue_mutex_);
    stop_ = true;
  }
  condition_.notify_all();
  for (std::thread &worker : workers_)
    worker.join();
}

} // namespace server
