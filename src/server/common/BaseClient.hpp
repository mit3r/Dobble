#include <iostream>
#include <string>
#include <vector>
#include <cstring>
#include <stdexcept>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>     
#include <arpa/inet.h>
#include <functional>
#include <thread>
#pragma once
class BaseClient {
protected:
    int sock_fd = -1;
    bool connected = false;
    bool logged_in = false;
    std::function<void(int)> client_handler_func;


public:
    virtual ~BaseClient();
    virtual void connect() = 0;
    void disconnect();
    bool isConnected() const;
    void setClientHandler(std::function<void(int)> handler);
    int getSocket() const { return sock_fd; }
};

class UdsClient : public BaseClient {
private:
    std::string socket_path;

public:
    explicit UdsClient(const std::string& path);
    void connect() override;
};

class TcpClient : public BaseClient {
private:
    std::string ip_address;
    int port;

public:
    explicit TcpClient(const std::string& ip, int p);
    void connect() override;
};

