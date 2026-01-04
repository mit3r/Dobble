
#include "LobbyClientHandler.hpp"
#include "LobbyServerState.hpp"
#include "server/common/BaseServer.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

LobbyServerState g_lobby_server;

int main(int argc, const char* argv[])
{
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <bsd_port> <uds_socket_path> [public_ip]" << std::endl;
        std::cerr << "Example: " << argv[0] << " 5000 /tmp/lobby_uds_socket 146.59.16.213" << std::endl;
        return 1;
    }

    int bsd_port = std::stoi(argv[1]);
    std::string uds_path = argv[2];
    std::string public_ip = "127.0.0.1";
    
    if (argc >= 4) {
        public_ip = argv[3];
    }
    
    g_lobby_server.setPublicIP(public_ip);

    std::cout << "[LobbyServer] Starting with BSD port: " << bsd_port 
              << ", UDS path: " << uds_path 
              << ", Public IP: " << public_ip << std::endl;

    BsdServer bsd_server(bsd_port, "0.0.0.0");
    UdsServer uds_server(uds_path.c_str());
    
    g_uds_server = &uds_server;
    
    bsd_server.setClientHandler(tcp_client_handler);
    
    uds_server.setClientHandler(uds_client_handler);
    
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