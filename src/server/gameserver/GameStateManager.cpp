#include "GameStateManager.hpp"
#include <algorithm>
#include <stdexcept>
#include <iostream>

using namespace GameServerState;

/*
void GameStateManager::setGameName(const std::string& name) {
    game_info_.game_name = name;
}
*/

const std::string& GameStateManager::getGameName() const {
    return game_info_.game_name;
}

void GameStateManager::setMaxPlayers(int max) {
    game_info_.max_players = max;
}

int GameStateManager::getMaxPlayers() const {
    return game_info_.max_players;
}

void GameStateManager::setGameStatus(const std::string& status) {
    game_info_.status = status;
}

const std::string& GameStateManager::getGameStatus() const {
    return game_info_.status;
}

GameInfo& GameStateManager::getGameInfo() {
    return game_info_;
}

const GameInfo& GameStateManager::getGameInfo() const {
    return game_info_;
}

/*
void GameStateManager::startNewTurn(int turn_id) {
    current_turn_.turn_id = turn_id;
    current_turn_.is_active = true;
    current_turn_.winner_id = "";
    current_turn_.images_on_table.clear();
    current_turn_.clients_data.clear();
    current_turn_.scoreboard.clear();
}

void GameStateManager::endCurrentTurn(const std::string& winner_id) {
    current_turn_.is_active = false;
    current_turn_.winner_id = winner_id;
    current_turn_.past_turns.push_back(current_turn_.turn_id);
}
*/

/*
bool GameStateManager::isTurnActive() const {
    return current_turn_.is_active;
}

void GameStateManager::setImagesOnTable(const std::vector<int>& images) {
    current_turn_.images_on_table = images;
}

const std::vector<int>& GameStateManager::getImagesOnTable() const {
    return current_turn_.images_on_table;
}

void GameStateManager::updateScoreboard(const std::string& client_id, int score) {
    current_turn_.scoreboard[client_id] = score;
}

int GameStateManager::getScore(const std::string& client_id) const {
    auto it = current_turn_.scoreboard.find(client_id);
    return (it != current_turn_.scoreboard.end()) ? it->second : 0;
}
*/

TurnInfo& GameStateManager::getCurrentTurn() {
    return current_turn_;
}

const TurnInfo& GameStateManager::getCurrentTurn() const {
    return current_turn_;
}

void GameStateManager::addPlayer(const std::string& client_id, const std::string& nickname, int socket_fd) {
    players_[client_id] = PlayerInfo(client_id, nickname, socket_fd);
    game_info_.player_nicknames.push_back(nickname);
    game_info_.players_count = static_cast<int>(players_.size());
}

void GameStateManager::removePlayer(const std::string& client_id) {
    players_.erase(client_id);
    game_info_.players_count = static_cast<int>(players_.size());
}

bool GameStateManager::hasPlayer(const std::string& client_id) const {
    return players_.find(client_id) != players_.end();
}

void GameStateManager::setPlayerImages(const std::string& client_id, const std::vector<int>& images) {
    if (players_.find(client_id) != players_.end()) {
        players_[client_id].images = images;
        current_turn_.clients_data[client_id] = images;
    }
}

void GameStateManager::updatePlayerScore(const std::string& client_id, int points, int mistakes) {
    if (players_.find(client_id) != players_.end()) {
        players_[client_id].points += points;
        players_[client_id].mistakes += mistakes;
        players_[client_id].score = players_[client_id].points - players_[client_id].mistakes;
    }
}

/*
void GameStateManager::setPlayerRank(const std::string& client_id, int rank) {
    if (players_.find(client_id) != players_.end()) {
        players_[client_id].rank = rank;
    }
}
*/

void GameStateManager::updatePlayerPing(const std::string& client_id) {
    if (players_.find(client_id) != players_.end()) {
        players_[client_id].last_ping_timestamp = std::chrono::steady_clock::now();
    }
}

PlayerInfo& GameStateManager::getPlayer(const std::string& client_id) {
    auto it = players_.find(client_id);
    if (it == players_.end()) {
        std::cerr << "[ERROR] Player not found: " << client_id << std::endl;
        static PlayerInfo empty_player;
        return empty_player;
    }
    return it->second;
}

const PlayerInfo& GameStateManager::getPlayer(const std::string& client_id) const {
    auto it = players_.find(client_id);
    if (it == players_.end()) {
        std::cerr << "[ERROR] Player not found: " << client_id << std::endl;
        static const PlayerInfo empty_player;
        return empty_player;
    }
    return it->second;
}

std::vector<std::string> GameStateManager::getAllPlayerIds() const {
    std::vector<std::string> ids;
    ids.reserve(players_.size());
    for (const auto& [client_id, _] : players_) {
        ids.push_back(client_id);
    }
    return ids;
}

int GameStateManager::getPlayerCount() const {
    return static_cast<int>(players_.size());
}

std::list<PlayerGameInfo> GameStateManager::getPlayersGameInfo() const {
    std::list<PlayerGameInfo> players_info;
    
    for (const auto& [client_id, player] : players_) {
        PlayerGameInfo info;
        info.nickname = player.nickname;
        info.score = player.score;
        info.points = player.points;
        info.mistakes = player.mistakes;
        
        if (!player.images.empty()) {
            info.cardId = player.images[0];
        } else {
            info.cardId = -1;
        }
        std::cout << "[GameStateManager] Player " << player.nickname 
                  << " - CardId: " << info.cardId << ", Score: " << info.score 
                  << ", Points: " << info.points << ", Mistakes: " << info.mistakes << std::endl;

        
        players_info.push_back(info);
    }
    
    return players_info;
}

std::vector<std::string> GameStateManager::getInactivePlayers(std::chrono::seconds timeout) const {
    std::vector<std::string> inactive_players;
    auto now = std::chrono::steady_clock::now();
    
    for (const auto& [client_id, player] : players_) {
        auto time_since_ping = std::chrono::duration_cast<std::chrono::seconds>(now - player.last_ping_timestamp);
        if (time_since_ping > timeout) {
            inactive_players.push_back(client_id);
        }
    }
    
    return inactive_players;
}

bool GameStateManager::isGameReady() const {
    return game_info_.players_count >= 2 && 
           game_info_.players_count <= game_info_.max_players;
}

bool GameStateManager::isGameFull() const {
    return game_info_.players_count >= game_info_.max_players;
}