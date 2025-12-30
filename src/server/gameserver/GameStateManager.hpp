#ifndef GAME_STATE_MANAGER_HPP
#define GAME_STATE_MANAGER_HPP

#include "../common/ServerUtils.hpp"
#include <vector>
#include <string>
#include <map>

namespace GameServerState {

struct GameInfo {
    std::string game_name;
    int players_count;
    int max_players;
    std::vector<std::string> player_nicknames;
    std::string status;
    
    GameInfo() : players_count(-1), max_players(-1), status("init") {}
};

struct PlayerInfo {
    int client_id;
    std::vector<int> images;
    int score;
    int points;
    int mistakes;
    int rank;
    
    PlayerInfo() : client_id(-1), score(0), points(0), mistakes(0), rank(0) {}
    explicit PlayerInfo(int id) : client_id(id), score(0), points(0), mistakes(0), rank(0) {}
};

struct TurnInfo {
    int turn_id;
    bool is_active;
    std::map<int, std::vector<int>> clients_data;  
    std::vector<int> images_on_table;
    std::map<int, int> scoreboard;  
    std::vector<int> past_turns;
    int winner_id;
    
    TurnInfo() : turn_id(0), is_active(false), winner_id(-1) {}
};

} // namespace GameServerState

class GameStateManager : public ServerStateManager {
private:
    GameServerState::GameInfo game_info_;
    GameServerState::TurnInfo current_turn_;
    std::map<int, GameServerState::PlayerInfo> players_;  
    
public:
    GameStateManager() = default;
    void setGameName(const std::string& name);
    std::string getGameName() const;
    
    void setMaxPlayers(int max);
    int getMaxPlayers() const;
    
    void setGameStatus(const std::string& status);
    std::string getGameStatus() const;
    
    GameServerState::GameInfo& getGameInfo();
    const GameServerState::GameInfo& getGameInfo() const;

    void startNewTurn(int turn_id);
    void endCurrentTurn(int winner_id);
    bool isTurnActive() const;
    
    void setImagesOnTable(const std::vector<int>& images);
    const std::vector<int>& getImagesOnTable() const;
    
    void updateScoreboard(int client_id, int score);
    int getScore(int client_id) const;
    
    GameServerState::TurnInfo& getCurrentTurn();
    const GameServerState::TurnInfo& getCurrentTurn() const;
    
    void addPlayer(int client_id, const std::string& nickname);
    void removePlayer(int client_id);
    bool hasPlayer(int client_id) const;
    
    void setPlayerImages(int client_id, const std::vector<int>& images);
    void updatePlayerScore(int client_id, int points, int mistakes);
    void setPlayerRank(int client_id, int rank);
    
    GameServerState::PlayerInfo& getPlayer(int client_id);
    const GameServerState::PlayerInfo& getPlayer(int client_id) const;
    
    std::vector<int> getAllPlayerIds() const;
    int getPlayerCount() const;
    
    bool isGameReady() const;
    bool isGameFull() const;
};

#endif // GAME_STATE_MANAGER_HPP
