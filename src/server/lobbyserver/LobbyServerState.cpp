#include <server/lobbyserver/LobbyServerState.hpp>

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

void LobbyServerState::setUDSPath(const std::string& path) {
    uds_path = path;
}

std::string LobbyServerState::getUDSPath() const {
    return uds_path;
}
