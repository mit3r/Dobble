#include "GameLogic.hpp"
#include <iostream>
#include <algorithm>
#include <sstream>

GameLogic::GameLogic(GameStateManager& manager) 
    : state_manager_(manager), current_deck_index_(0), gen_(rd_()) {
}

bool GameLogic::CheckSymbol(int cardId1, int cardId2, int symbolId) const {
    if (cardId1 < 0 || cardId1 >= 57 || cardId2 < 0 || cardId2 >= 57) {
        return false;
    }
    
    bool found_on_card1 = false;
    bool found_on_card2 = false;
    
    for (int i = 0; i < 8; i++) {
        if (cards[cardId1][i] == symbolId) {
            found_on_card1 = true;
            break;
        }
    }
    
    for (int i = 0; i < 8; i++) {
        if (cards[cardId2][i] == symbolId) {
            found_on_card2 = true;
            break;
        }
    }
    
    return found_on_card1 && found_on_card2;
}

void GameLogic::InitializeGame() {
    std::cout << "[GameLogic] Initializing game..." << std::endl;
    
    deck_indices_.clear();
    for (int i = 0; i < 57; i++) {
        deck_indices_.push_back(i);
    }
    std::shuffle(deck_indices_.begin(), deck_indices_.end(), gen_);
    
    current_deck_index_ = 0;
    
    auto& turn = state_manager_.getCurrentTurn();
    
    std::stringstream ss;
    ss << "turn_" << std::time(nullptr) << "_" << (gen_() % 10000);
    turn.turn_id = 0;
    
    turn.is_active = true;
    turn.images_on_table.clear();
    turn.images_on_table.push_back(deck_indices_[0]);
    turn.winner_id = "";
    turn.past_turns.clear();
    
    auto player_ids = state_manager_.getAllPlayerIds();
    int next_card_index = 1;
    for (const auto& client_id : player_ids) {
        std::vector<int> player_cards;
        if (next_card_index < static_cast<int>(deck_indices_.size())) {
            player_cards.push_back(deck_indices_[next_card_index]);
            next_card_index++;
        }
        turn.clients_data[client_id] = player_cards;
        state_manager_.setPlayerImages(client_id, player_cards);
    }
    
    current_deck_index_ = next_card_index - 1;
    
    state_manager_.setGameStatus("GAME_ACTIVE");
    
    std::cout << "[GameLogic] Game initialized. Top card: " << deck_indices_[0] 
              << ", players have cards from index 1 to " << (next_card_index - 1) << std::endl;
}

GameLogic::MatchResult GameLogic::ProcessMatch(const std::string& client_id, int turn_id, int symbol_id) {
    MatchResult result;
    result.success = false;
    result.points_awarded = 0;
    
    auto& turn = state_manager_.getCurrentTurn();
    
    if (turn.turn_id != turn_id) {
        result.message = "TOO_LATE";
        std::cout << "[GameLogic] Client " << client_id << " sent wrong turn_id (expected " 
                  << turn.turn_id << ", got " << turn_id << ")" << std::endl;
        return result;
    }
    
    if (!turn.is_active) {
        result.message = "TOO_LATE";
        std::cout << "[GameLogic] Client " << client_id << " too late - round already finished" << std::endl;
        return result;
    }
    
    if (turn.images_on_table.empty()) {
        result.message = "NO_CARD_ON_TABLE";
        return result;
    }
    int top_card_id = turn.images_on_table[0];
    
    if (turn.clients_data.find(client_id) == turn.clients_data.end() || turn.clients_data[client_id].empty()) {
        result.message = "NO_PLAYER_CARD";
        std::cout << "[GameLogic] Client " << client_id << " has no cards" << std::endl;
        return result;
    }
    
    int player_card_id = turn.clients_data[client_id][0];
    
    bool is_correct = CheckSymbol(top_card_id, player_card_id, symbol_id);
    
    if (is_correct) {
        std::cout << "[GameLogic] Client " << client_id << " found correct symbol: " << symbol_id << std::endl;
        
        turn.is_active = false;
        turn.winner_id = client_id;
        
        state_manager_.updatePlayerScore(client_id, 10, 0);
        
        turn.clients_data[client_id].clear();
        turn.clients_data[client_id].push_back(top_card_id);
        state_manager_.setPlayerImages(client_id, turn.clients_data[client_id]);
        
        std::cout << "[GameLogic] Client " << client_id << " received top card: " << top_card_id << std::endl;
        
        result.success = true;
        result.message = "CORRECT";
        result.points_awarded = 10;
        
        turn.past_turns.push_back(turn.turn_id);
        
    } else {
        std::cout << "[GameLogic] Client " << client_id << " wrong symbol: " << symbol_id << std::endl;
        
        state_manager_.updatePlayerScore(client_id, 0, 1);
        
        result.success = false;
        result.message = "INCORRECT";
        result.points_awarded = 0;
    }
    
    return result;
}

void GameLogic::NextRound() {
    std::cout << "[GameLogic] Starting next round..." << std::endl;
    
    current_deck_index_++;
    
    auto& turn = state_manager_.getCurrentTurn();
    
    if (current_deck_index_ >= MAX_ROUNDS) {
        std::cout << "[GameLogic] Game Over - reached " << MAX_ROUNDS << " rounds" << std::endl;
        state_manager_.setGameStatus("GAME_OVER");
        turn.is_active = false;
        CalculateRanks();
        return;
    }
    
    turn.images_on_table.clear();
    turn.images_on_table.push_back(deck_indices_[current_deck_index_]);
    
    turn.turn_id++;
    
    turn.is_active = true;
    turn.winner_id = "";
    
    std::cout << "[GameLogic] Next card: " << deck_indices_[current_deck_index_] << std::endl;
}

bool GameLogic::IsGameOver() const {
    return state_manager_.getGameStatus() == "GAME_OVER";
}

void GameLogic::CalculateRanks() {
    std::cout << "[GameLogic] Calculating ranks..." << std::endl;
    
    auto player_ids = state_manager_.getAllPlayerIds();
    std::vector<std::pair<std::string, int>> scores;
    for (const auto& client_id : player_ids) {
        auto player = state_manager_.getPlayer(client_id);
        scores.push_back({client_id, player.score});
    }
    
    std::sort(scores.begin(), scores.end(), 
              [](const auto& a, const auto& b) { return a.second > b.second; });
    
    int rank = 1;
    for (const auto& [client_id, score] : scores) {
        state_manager_.setPlayerRank(client_id, rank);
        std::cout << "[GameLogic] Rank " << rank << ": Client " << client_id 
                  << " with score " << score << std::endl;
        rank++;
    }
}

int GameLogic::GetTopCardId() const {
    if (current_deck_index_ >= 57) return -1;
    return deck_indices_[current_deck_index_];
}

const std::vector<int>& GameLogic::GetCardSymbols(int cardId) const {
    static std::vector<int> symbols;
    symbols.clear();
    
    if (cardId >= 0 && cardId < 57) {
        for (int i = 0; i < 8; i++) {
            symbols.push_back(cards[cardId][i]);
        }
    }
    
    return symbols;
}
