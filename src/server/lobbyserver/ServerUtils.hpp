#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <memory>
#include <string>
#include <vector>
#include <mutex>

struct Client {
    int socket;
    std::string nickname;
    bool logged_in;
};

struct GameServer {
    int socket;
    std::string server_id;
    bool registered;
};

struct ServerState {
    std::vector<std::shared_ptr<Client>> clients;
    std::mutex clients_mutex;
};

extern ServerState g_server;

std::shared_ptr<Client> findClientBySocket(ServerState& state, int targetSocket);
std::shared_ptr<Client> findClientByNickname(ServerState& state, const std::string& targetNickname);

#endif 
