#ifndef LOBBY_SERVER_STATE_HPP
#define LOBBY_SERVER_STATE_HPP

#include <server/ServerUtils.hpp>

class LobbyServerState : public ServerStateManager {
private:
    std::vector<std::shared_ptr<GameServer>> game_servers;
    std::mutex game_servers_mutex;
    std::string uds_path;

public:
    LobbyServerState();

    void addGameServer(std::shared_ptr<GameServer> server);
    void removeGameServer(std::shared_ptr<GameServer> server);
    std::shared_ptr<GameServer> findGameServerById(const std::string& server_id);
    size_t getGameServerCount() const;

    void setUDSPath(const std::string& path);
    std::string getUDSPath() const;
};

#endif 
