#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <unistd.h>

#include <algorithm>
#include <cstring>
#include <iostream>
#include <mutex>
#include <nlohmann/json.hpp>
#include <optional>
#include <server/BaseServer.hpp>
#include <server/ServerUtils.hpp>
#include <server/gameserver/GameClientHandler.hpp>
#include <string>
#include <thread>
#include <variant>

ServerStateManager g_game_server;

int main(int argc, char* argv[]) {
  if (argc < 2) {
    std::cerr << "Usage: " << argv[0] << " <port>" << std::endl;
    return 1;
  }
  int port = std::stoi(argv[1]);
  BsdServer bsd_server(port, "127.0.0.1");
  UdsServer uds_server("/tmp/lobby_uds_socket");

  bsd_server.setClientHandler(client_handler);
  uds_server.setClientHandler(client_handler);

  std::thread bsd_thread([&bsd_server]() {
    bsd_server.run();
  });

  std::thread uds_thread([&uds_server]() {
    uds_server.run();
  });

  bsd_thread.join();
  uds_thread.join();

  return 0;
}