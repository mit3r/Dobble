#ifndef GAME_STATE_MANAGER_HPP
#define GAME_STATE_MANAGER_HPP

#include "../common/ServerUtils.hpp"
#include <protocol/lobby/CommonProtocolStructs.hpp>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <chrono>

namespace GameServerState {

struct GameInfo {
    std::string game_id;
    std::string game_name;
    int players_count;
    int max_players;
    std::vector<std::string> player_nicknames;
    std::string status;
    
    GameInfo() : players_count(-1), max_players(-1), status("init") {}
};

struct PlayerInfo {
    std::string client_id;
    std::string nickname;
    std::vector<int> images;
    int score;
    int points;
    int mistakes;
    int rank;
    int socket_fd;
    std::chrono::steady_clock::time_point last_ping_timestamp;

    PlayerInfo() : client_id("-1"), nickname(""), score(0), points(0), mistakes(0), rank(0), 
                   socket_fd(-1), last_ping_timestamp(std::chrono::steady_clock::now()) {}
    explicit PlayerInfo(std::string id, std::string nick = "", int sock = -1) 
        : client_id(id), nickname(nick), score(0), points(0), mistakes(0), rank(0),
          socket_fd(sock), last_ping_timestamp(std::chrono::steady_clock::now()) {}
};

struct TurnInfo {
    int turn_id;
    bool is_active;
    std::map<std::string, std::vector<int>> clients_data;  
    std::vector<int> images_on_table;
    std::map<std::string, int> scoreboard;  
    std::vector<int> past_turns;
    std::string winner_id;
    
    TurnInfo() : turn_id(0), is_active(false), winner_id("") {}
};

} // namespace GameServerState

class GameStateManager : public ServerStateManager {
private:
    GameServerState::GameInfo game_info_;
    GameServerState::TurnInfo current_turn_;
    std::map<std::string, GameServerState::PlayerInfo> players_;  
    
public:
    GameStateManager() = default;
    // void setGameName(const std::string& name);
    const std::string& getGameName() const;
    
    void setMaxPlayers(int max);
    int getMaxPlayers() const;
    
    void setGameStatus(const std::string& status);
    const std::string& getGameStatus() const;
    
    GameServerState::GameInfo& getGameInfo();
    const GameServerState::GameInfo& getGameInfo() const;

    // void startNewTurn(int turn_id);
    // void endCurrentTurn(const std::string& winner_id);
    // bool isTurnActive() const;
    
    // void setImagesOnTable(const std::vector<int>& images);
    // const std::vector<int>& getImagesOnTable() const;
    
    // void updateScoreboard(const std::string& client_id, int score);
    // int getScore(const std::string& client_id) const;

    GameServerState::TurnInfo& getCurrentTurn();
    const GameServerState::TurnInfo& getCurrentTurn() const;
    
    void addPlayer(const std::string& client_id, const std::string& nickname, int socket_fd);
    void removePlayer(const std::string& client_id);
    bool hasPlayer(const std::string& client_id) const;

    void setPlayerImages(const std::string& client_id, const std::vector<int>& images);
    void updatePlayerScore(const std::string& client_id, int points, int mistakes);
    // void setPlayerRank(const std::string& client_id, int rank);
    void updatePlayerPing(const std::string& client_id);

    GameServerState::PlayerInfo& getPlayer(const std::string& client_id);
    const GameServerState::PlayerInfo& getPlayer(const std::string& client_id) const;

    std::vector<std::string> getAllPlayerIds() const;
    int getPlayerCount() const;
    
    std::list<PlayerGameInfo> getPlayersGameInfo() const;
    std::vector<std::string> getInactivePlayers(std::chrono::seconds timeout) const;

    void setGameId(const std::string& game_id) { game_info_.game_id = game_id; }

    bool isGameReady() const;
    bool isGameFull() const;
};

#endif 
