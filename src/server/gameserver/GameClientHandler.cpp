#include "./GameClientHandler.hpp"

extern ServerStateManager g_game_server;

void client_handler(int client_sock) {
  server_handle_client(client_sock, g_game_server, [](int sock, LobbyCommandFactory& factory) {
    return ServerCommandVisitor(sock, factory);
  });
}
