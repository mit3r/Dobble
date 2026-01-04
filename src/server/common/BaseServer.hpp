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
#include <vector>
#include <memory>
#include <mutex>

#pragma once

struct GameServer;

class BaseServer {
  protected:
  int server_fd;
  std::function<void(int)> client_handler_func;

  public:
  BaseServer();
  virtual ~BaseServer();

  void setClientHandler(std::function<void(int)> handler);
  virtual int run() = 0;  // Pure virtual - must be implemented by derived classes
  void shutdown();
};

class BsdServer : public BaseServer {
  private:
  int port;
  const char* host;

  public:
  explicit BsdServer(int port = 1500, const char* host = "127.0.0.1");
  int run() override;
};

class UdsServer : public BaseServer {
  private:
  const char* socket_path;

  public:
  std::vector<std::shared_ptr<GameServer>> game_servers;
  std::mutex game_servers_mutex;

  explicit UdsServer(const char* socket_path = "/tmp/uds_socket");
  int run() override;
  
  void addGameServer(std::shared_ptr<GameServer> game);
  void removeGameServer(const int& client_sock);
  std::shared_ptr<GameServer> getGameServerById(const std::string& server_id);
  std::shared_ptr<GameServer> getGameServerBySocket(int socket);
  std::vector<std::shared_ptr<GameServer>> getAllGameServers();
};

extern UdsServer* g_uds_server;
