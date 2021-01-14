#include "web_server.h"

#include <arpa/inet.h>
#include <netdb.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <sstream>

namespace server {

web_server::web_server(const std::string &address_, int port_, size_t threads_)
    : address(address_), port(port_), t_pool(threads_) {}

bool web_server::init() {
  sockaddr_in addr;
  bzero(&addr, sizeof(addr));

  addr.sin_family = AF_INET;
  addr.sin_port = htons(port);
  addr.sin_addr.s_addr = inet_addr(address.c_str());

  sock_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

  int enable = 1;
  if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0) {
    std::cerr << "setsockopt(SO_REUSEADDR) failed" << std::endl;
  }

  if (sock_fd == -1) {
    std::cerr << "Socket creation error" << std::endl;
    return false;
  }

  if (bind(sock_fd, (struct sockaddr *)&addr, sizeof(addr)) == -1) {
    std::cerr << "Bind error" << std::endl;
    close(sock_fd);
    return false;
  }

  if (listen(sock_fd, 5 /*length of connections queue*/) == -1) {
    std::cerr << "Listen error" << std::endl;
    close(sock_fd);
    return false;
  }

  return true;
}

void web_server::run() {
  while (true) {
    sockaddr_in client;
    socklen_t client_len = sizeof(client);
    auto client_fd =
        accept(sock_fd, reinterpret_cast<sockaddr *>(&client), &client_len);
    std::cout << "Client with" << inet_ntoa(client.sin_addr) << " connected"
              << std::endl;

    t_pool.enqueue([&] { clientHandler(client_fd); });
  }
}

void web_server::clientHandler(int client_fd) {
  char buf[1024];

  std::stringstream wsss;
  std::string resp =
      "<html><body><center><h1>Hello World!</h1></center></body></html>";
  wsss << "HTTP/1.1 200 OK\r\n"
       << "Content-Type: text/html; charset=utf-8\r\n"
       << "Content-Length: " << resp.length() << "\r\n"
       << "\r\n";
  wsss.write(resp.data(), resp.length());

  int read = recv(client_fd, buf, 1024, 0);

  send(client_fd, wsss.str().data(), wsss.str().length(), 0);

  close(client_fd);
}

} // namespace server
