#include "BaseServer.hpp"
#include <iostream>
#include <thread>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <cstring>

BaseServer::BaseServer() : server_fd(-1) {}

BaseServer::~BaseServer() {
    shutdown();
}

void BaseServer::setClientHandler(std::function<void(int)> handler) {
    client_handler_func = handler;
}

void BaseServer::shutdown() {
    if (server_fd >= 0) {
        close(server_fd);
        server_fd = -1;
    }
}

BsdServer::BsdServer(int port, const char* host) 
    : BaseServer(), port(port), host(host) {}

int BsdServer::run() {
    server_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server_fd < 0) {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    struct sockaddr_in socket_addr;
    socket_addr.sin_family = AF_INET;
    socket_addr.sin_port = htons(port);
    socket_addr.sin_addr.s_addr = inet_addr(host);

    if (bind(server_fd, (struct sockaddr*)&socket_addr, sizeof(socket_addr)) < 0) {
        perror("bind");
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("listen");
        return 1;
    }

    std::cout << "[BSD] Serwer nasłuchuje na " << host << ":" << port << "..." << std::endl;

    while (true) {
        int client_sock = accept(server_fd, NULL, NULL);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }
        
        if (client_handler_func) {
            std::thread t(client_handler_func, client_sock);
            t.detach();
        } else {
            std::cerr << "[ERROR] Client handler not set!" << std::endl;
            close(client_sock);
        }
    }

    close(server_fd);
    return 0;
}

UdsServer::UdsServer(const char* socket_path) 
    : BaseServer(), socket_path(socket_path) {}

int UdsServer::run() {
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("UDS socket failed");
        return 1;
    }

    struct sockaddr_un address;
    address.sun_family = AF_UNIX;
    strncpy(address.sun_path, socket_path, sizeof(address.sun_path) - 1);

    unlink(socket_path);

    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("UDS bind failed");
        return 1;
    }

    if (listen(server_fd, 10) < 0) {
        perror("UDS listen failed");
        return 1;
    }

    std::cout << "[UDS] Serwer nasłuchuje na ścieżce: " << socket_path << "..." << std::endl;

    while (true) {
        int client_sock = accept(server_fd, NULL, NULL);
        if (client_sock < 0) {
            perror("UDS accept failed");
            continue;
        }

        std::cout << "[UDS] Nowe połączenie lokalne!" << std::endl;
        
        if (client_handler_func) {
            std::thread t(client_handler_func, client_sock);
            t.detach();
        } else {
            std::cerr << "[ERROR] Client handler not set!" << std::endl;
            close(client_sock);
        }
    }

    close(server_fd);
    unlink(socket_path);
    return 0;
}
