#include "GameStateManager.hpp"
#include <algorithm>
#include <stdexcept>

using namespace GameServerState;

// Game info management
void GameStateManager::setGameName(const std::string& name) {
    game_info_.game_name = name;
}

std::string GameStateManager::getGameName() const {
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

std::string GameStateManager::getGameStatus() const {
    return game_info_.status;
}

GameInfo& GameStateManager::getGameInfo() {
    return game_info_;
}

const GameInfo& GameStateManager::getGameInfo() const {
    return game_info_;
}

void GameStateManager::startNewTurn(int turn_id) {
    current_turn_.turn_id = turn_id;
    current_turn_.is_active = true;
    current_turn_.winner_id = -1;
    current_turn_.images_on_table.clear();
    current_turn_.clients_data.clear();
    current_turn_.scoreboard.clear();
}

void GameStateManager::endCurrentTurn(int winner_id) {
    current_turn_.is_active = false;
    current_turn_.winner_id = winner_id;
    current_turn_.past_turns.push_back(current_turn_.turn_id);
}

bool GameStateManager::isTurnActive() const {
    return current_turn_.is_active;
}

void GameStateManager::setImagesOnTable(const std::vector<int>& images) {
    current_turn_.images_on_table = images;
}

const std::vector<int>& GameStateManager::getImagesOnTable() const {
    return current_turn_.images_on_table;
}

void GameStateManager::updateScoreboard(int client_id, int score) {
    current_turn_.scoreboard[client_id] = score;
}

int GameStateManager::getScore(int client_id) const {
    auto it = current_turn_.scoreboard.find(client_id);
    return (it != current_turn_.scoreboard.end()) ? it->second : 0;
}

TurnInfo& GameStateManager::getCurrentTurn() {
    return current_turn_;
}

const TurnInfo& GameStateManager::getCurrentTurn() const {
    return current_turn_;
}

void GameStateManager::addPlayer(int client_id, const std::string& nickname) {
    players_[client_id] = PlayerInfo(client_id);
    game_info_.player_nicknames.push_back(nickname);
    game_info_.players_count = static_cast<int>(players_.size());
}

void GameStateManager::removePlayer(int client_id) {
    players_.erase(client_id);
    game_info_.players_count = static_cast<int>(players_.size());
}

bool GameStateManager::hasPlayer(int client_id) const {
    return players_.find(client_id) != players_.end();
}

void GameStateManager::setPlayerImages(int client_id, const std::vector<int>& images) {
    if (players_.find(client_id) != players_.end()) {
        players_[client_id].images = images;
        current_turn_.clients_data[client_id] = images;
    }
}

void GameStateManager::updatePlayerScore(int client_id, int points, int mistakes) {
    if (players_.find(client_id) != players_.end()) {
        players_[client_id].points += points;
        players_[client_id].mistakes += mistakes;
        players_[client_id].score = players_[client_id].points - players_[client_id].mistakes;
    }
}

void GameStateManager::setPlayerRank(int client_id, int rank) {
    if (players_.find(client_id) != players_.end()) {
        players_[client_id].rank = rank;
    }
}

PlayerInfo& GameStateManager::getPlayer(int client_id) {
    auto it = players_.find(client_id);
    if (it == players_.end()) {
        throw std::runtime_error("Player not found: " + std::to_string(client_id));
    }
    return it->second;
}

const PlayerInfo& GameStateManager::getPlayer(int client_id) const {
    auto it = players_.find(client_id);
    if (it == players_.end()) {
        throw std::runtime_error("Player not found: " + std::to_string(client_id));
    }
    return it->second;
}

std::vector<int> GameStateManager::getAllPlayerIds() const {
    std::vector<int> ids;
    ids.reserve(players_.size());
    for (const auto& [client_id, _] : players_) {
        ids.push_back(client_id);
    }
    return ids;
}

int GameStateManager::getPlayerCount() const {
    return static_cast<int>(players_.size());
}

bool GameStateManager::isGameReady() const {
    return game_info_.players_count >= 2 && 
           game_info_.players_count <= game_info_.max_players;
}

bool GameStateManager::isGameFull() const {
    return game_info_.players_count >= game_info_.max_players;
}