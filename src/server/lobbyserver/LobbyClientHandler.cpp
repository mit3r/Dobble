#include "LobbyClientHandler.hpp"

extern LobbyServerState g_lobby_server;

void client_handler(int client_sock) {
  handle_client(client_sock, g_lobby_server, [](int sock, CommandFactory& factory) {
    return ServerCommandVisitor(sock, factory);
  });
}
