#pragma once

#include <string>

namespace server {

class web_server {
private:
  std::string address;
  int port;
  int sock_fd;

public:
  web_server(const std::string &address_, int port_);
  ~web_server() = default;

  bool init();

  void run();
};

} // namespace server
