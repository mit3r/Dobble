#include <server/lobbyserver/LobbyClientHandler.hpp>
#include <server/lobbyserver/LobbyServerState.hpp>
#include <server/lobbyserver/ServerCommandVisitor.hpp>
#include <server/ClientHandler.hpp>
#include "../ClientHandler.cpp"  

extern LobbyServerState g_lobby_server;

void client_handler(int client_sock)
{
    handle_client(client_sock, g_lobby_server, [](int sock, CommandFactory& factory) {
        return ServerCommandVisitor(sock, factory);
    });
}
