#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <mutex>
#include <algorithm>
#include <optional>
#include <cstring>
#include <variant>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <nlohmann/json.hpp>
#include <server/BaseServer.hpp>
#include <server/ServerUtils.hpp>
#include <server/gameserver/GameClientHandler.hpp>
#include <iostream>
#include <thread>

ServerStateManager g_game_server;

int main(int argc, char *argv[])
{
    if (argc < 4)
    {
        std::cerr << "Usage: " << argv[0] << " <server_id> <port> <lobby_uds_path>" << std::endl;
        return 1;
    }
    
    std::string server_id = argv[1];
    int port = std::stoi(argv[2]);
    std::string lobby_uds_path = argv[3];
    
    std::cout << "[GameServer] Starting with ID: " << server_id 
              << ", Port: " << port 
              << ", Lobby UDS: " << lobby_uds_path << std::endl;
    
    BsdServer bsd_server(port, "127.0.0.1");
    UdsServer uds_server("/tmp/game_server_uds_socket");
    
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