
#include "LobbyClientHandler.hpp"
#include "LobbyServerState.hpp"
#include "server/common/BaseServer.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

LobbyServerState g_lobby_server;

int main(int argc, char *argv[])
{
    BsdServer bsd_server(1500, "127.0.0.1");
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