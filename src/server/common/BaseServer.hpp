#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <cstring>
#include <functional>
#include <iostream>
#include <string>
#include <thread>

#pragma once

class BaseServer {
  protected:
  int server_fd;
  std::function<void(int)> client_handler_func;

  public:
  BaseServer();
  virtual ~BaseServer();

  void setClientHandler(std::function<void(int)> handler);
  virtual int run() = 0;  // Pure virtual - must be implemented by derived classes
  virtual void shutdown();
};

class BsdServer : public BaseServer {
  private:
  int port;
  const char* host;

  public:
  BsdServer(int port = 1500, const char* host = "127.0.0.1");
  int run() override;
};

class UdsServer : public BaseServer {
  private:
  const char* socket_path;

  public:
  UdsServer(const char* socket_path = "/tmp/uds_socket");
  int run() override;
};
