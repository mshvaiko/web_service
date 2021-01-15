#pragma once

#include <string>
#include <thread>

#include "thread_pool.h"

namespace server {

class web_server {
private:
  std::string address;
  int port;
  int sock_fd;
  thread_pool t_pool;

  void clientHandler(int client_fd);

public:
  web_server(const std::string &address_, int port_,
             size_t threads_ = std::thread::hardware_concurrency());
  ~web_server() = default;

  bool init();

  void run();
};

} // namespace server
