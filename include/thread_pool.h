#pragma once

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

namespace server {

class thread_pool {
private:
  // need to keep track of threads so we can join them
  std::vector<std::thread> workers_;
  // the task queue
  std::queue<std::function<void()>> tasks_;

  // synchronization
  std::mutex queue_mutex_;
  std::condition_variable condition_;
  bool stop_;

public:
  explicit thread_pool(size_t pool_size);
  ~thread_pool();

  // this object shouldn't be copied
  thread_pool(const thread_pool &) = delete;
  thread_pool &operator=(const thread_pool &) = delete;

  void enqueue(std::function<void()> task);
};

} // namespace server
