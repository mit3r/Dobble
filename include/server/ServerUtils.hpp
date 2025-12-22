// #ifndef SERVER_UTILS_HPP
// #define SERVER_UTILS_HPP

#include <sys/types.h>

#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#pragma once

struct Client {
    int socket;
    std::string nickname;
    std::string client_id;
    bool logged_in;
};

struct GameServer {
    int socket;
    std::string server_id;
    std::string servername;
    bool registered;
    pid_t process_pid;
    int port;
};

class ServerStateManager {
protected:
    std::vector<std::shared_ptr<Client>> clients;
    std::mutex clients_mutex;

public:
    ServerStateManager();
    virtual ~ServerStateManager();
    virtual void addClient(std::shared_ptr<Client> client);
    virtual void removeClient(std::shared_ptr<Client> client);
    virtual std::shared_ptr<Client> findClientBySocket(int targetSocket);
    virtual std::shared_ptr<Client> findClientByNickname(const std::string& targetNickname);
    size_t getClientCount() const;
    std::vector<std::shared_ptr<Client>> getClients();
};

int find_available_port();

// #endif
