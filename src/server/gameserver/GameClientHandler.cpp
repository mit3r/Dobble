#include "./GameClientHandler.hpp"
#include "GameStateManager.hpp"

extern GameStateManager g_game_server;

void tcp_client_handler(int client_sock) 
{
  GameCommandFactory factory;
  server_handle_client(client_sock, g_game_server, [](int sock, GameCommandFactory& factory) {
    return ServerCommandVisitor(sock, factory);
  }, factory);
}

void uds_client_handler(int client_sock) {
  ServersCommandFactory factory;
  registerServer(g_game_server, client_sock);
  client_handle_client(client_sock, g_game_server, [](int sock, ServersCommandFactory& factory) {
    return ServerCommandVisitor(sock, factory);
  }, factory);
}
