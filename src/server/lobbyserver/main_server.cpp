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
#include "../BaseServer.hpp"
#include "ServerUtils.hpp"
#include "LobbyClientHandler.hpp"
#include <iostream>
#include <thread>

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