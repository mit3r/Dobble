#include <server/gameserver/GameClientHandler.hpp>
#include <server/ServerUtils.hpp>
#include <server/gameserver/ServerCommandVisitor.hpp>
#include <server/ClientHandler.hpp>
#include "../ClientHandler.cpp"  

extern ServerStateManager g_game_server;

void client_handler(int client_sock)
{
    handle_client(client_sock, g_game_server, [](int sock, CommandFactory& factory) {
        return ServerCommandVisitor(sock, factory);
    });
}
