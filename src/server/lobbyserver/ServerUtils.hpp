#ifndef SERVER_UTILS_HPP
#define SERVER_UTILS_HPP

#include <memory>
#include <string>
#include <vector>
#include <mutex>
#include <algorithm>

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

class LobbyServerState : public ServerStateManager {
private:
    std::vector<std::shared_ptr<GameServer>> game_servers;
    std::mutex game_servers_mutex;

public:
    LobbyServerState();
    void addGameServer(std::shared_ptr<GameServer> server);
    void removeGameServer(std::shared_ptr<GameServer> server);
    std::shared_ptr<GameServer> findGameServerById(const std::string& server_id);
    size_t getGameServerCount() const;
};

extern LobbyServerState g_lobby_server;

#endif 
