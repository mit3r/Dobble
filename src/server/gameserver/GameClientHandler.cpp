#include "./GameClientHandler.hpp"

extern ServerStateManager g_game_server;

// void client_handler(int client_sock) {
//   LobbyCommandFactory factory;
//   server_handle_client(client_sock, g_game_server, [](int sock, LobbyCommandFactory& factory) {
//     return ServerCommandVisitor(sock, factory);
//   }, factory);
// }

void uds_client_handler(int client_sock) {
  ServersCommandFactory factory;
  registerServer(g_game_server, client_sock);
  client_handle_client(client_sock, g_game_server, [](int sock, ServersCommandFactory& factory) {
    return ServerCommandVisitor(sock, factory);
  }, factory);
}
