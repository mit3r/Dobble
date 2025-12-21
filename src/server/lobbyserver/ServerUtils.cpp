#include "ServerUtils.hpp"

ServerStateManager::ServerStateManager() {}

ServerStateManager::~ServerStateManager() {}

void ServerStateManager::addClient(std::shared_ptr<Client> client) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    clients.push_back(client);
}

void ServerStateManager::removeClient(std::shared_ptr<Client> client) {
    std::lock_guard<std::mutex> lock(clients_mutex);
    auto it = std::remove(clients.begin(), clients.end(), client);
    clients.erase(it, clients.end());
}

std::shared_ptr<Client> ServerStateManager::findClientBySocket(int targetSocket) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    auto it = std::find_if(clients.begin(), clients.end(),
                           [targetSocket](const std::shared_ptr<Client>& c) {
                               return c->socket == targetSocket;
                           });

    if (it != clients.end()) {
        return *it;
    }

    return nullptr;
}

std::shared_ptr<Client> ServerStateManager::findClientByNickname(const std::string& targetNickname) {
    std::lock_guard<std::mutex> lock(clients_mutex);

    auto it = std::find_if(clients.begin(), clients.end(),
                           [targetNickname](const std::shared_ptr<Client>& c) {
                               return c->nickname == targetNickname;
                           });

    if (it != clients.end()) {
        return *it;
    }

    return nullptr;
}

size_t ServerStateManager::getClientCount() const {
    return clients.size();
}

std::vector<std::shared_ptr<Client>> ServerStateManager::getClients() {
    std::lock_guard<std::mutex> lock(clients_mutex);
    return clients;
}

LobbyServerState::LobbyServerState() : ServerStateManager() {}

void LobbyServerState::addGameServer(std::shared_ptr<GameServer> server) {
    std::lock_guard<std::mutex> lock(game_servers_mutex);
    game_servers.push_back(server);
}

void LobbyServerState::removeGameServer(std::shared_ptr<GameServer> server) {
    std::lock_guard<std::mutex> lock(game_servers_mutex);
    auto it = std::remove(game_servers.begin(), game_servers.end(), server);
    game_servers.erase(it, game_servers.end());
}

std::shared_ptr<GameServer> LobbyServerState::findGameServerById(const std::string& server_id) {
    std::lock_guard<std::mutex> lock(game_servers_mutex);

    auto it = std::find_if(game_servers.begin(), game_servers.end(),
                           [&server_id](const std::shared_ptr<GameServer>& gs) {
                               return gs->server_id == server_id;
                           });

    if (it != game_servers.end()) {
        return *it;
    }

    return nullptr;
}

size_t LobbyServerState::getGameServerCount() const {
    return game_servers.size();
}

LobbyServerState g_lobby_server;
