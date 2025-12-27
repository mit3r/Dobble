#include "LobbyClientHandler.hpp"

extern LobbyServerState g_lobby_server;

void tcp_client_handler(int client_sock) {
  LobbyCommandFactory factory;
  server_handle_client(client_sock, g_lobby_server, [](int sock, LobbyCommandFactory& factory) {
    return ServerCommandVisitor(sock, factory);
  }, factory);
}

void uds_client_handler(int client_sock) {
  
  std::cout << "[UDS Handler] Game server połączony, socket: " << client_sock << std::endl;

  ServersCommandFactory factory;
  server_handle_client(client_sock, g_lobby_server, [](int sock, ServersCommandFactory& factory) {
    return ServerCommandVisitor(sock, factory);
  }, factory);

  
  close(client_sock);
}
