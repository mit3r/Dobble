#ifndef GAME_ENUMS_HPP
#define GAME_ENUMS_HPP

#include <string>

namespace GameEnums {

enum class GameStatus {
    INIT,
    WAITING,
    GAME_ACTIVE,
    GAME_OVER
};

enum class MatchResult {
    CORRECT,
    INCORRECT,
    TOO_LATE,
    NO_CARD_ON_TABLE,
    NO_PLAYER_CARD,
    INVALID_REQUEST
};

inline std::string toString(GameStatus status) {
    switch (status) {
        case GameStatus::INIT: return "INIT";
        case GameStatus::WAITING: return "WAITING";
        case GameStatus::GAME_ACTIVE: return "GAME_ACTIVE";
        case GameStatus::GAME_OVER: return "GAME_OVER";
        default: return "unknown";
    }
}

inline std::string toString(MatchResult result) {
    switch (result) {
        case MatchResult::CORRECT: return "CORRECT";
        case MatchResult::INCORRECT: return "INCORRECT";
        case MatchResult::TOO_LATE: return "TOO_LATE";
        case MatchResult::NO_CARD_ON_TABLE: return "NO_CARD_ON_TABLE";
        case MatchResult::NO_PLAYER_CARD: return "NO_PLAYER_CARD";
        case MatchResult::INVALID_REQUEST: return "INVALID_REQUEST";
        default: return "UNKNOWN";
    }
}

inline GameStatus toGameStatus(const std::string& str) {
    if (str == "init") return GameStatus::INIT;
    if (str == "waiting") return GameStatus::WAITING;
    if (str == "GAME_ACTIVE") return GameStatus::GAME_ACTIVE;
    if (str == "GAME_OVER") return GameStatus::GAME_OVER;
    return GameStatus::INIT;
}

inline MatchResult toMatchResult(const std::string& str) {
    if (str == "CORRECT") return MatchResult::CORRECT;
    if (str == "INCORRECT") return MatchResult::INCORRECT;
    if (str == "TOO_LATE") return MatchResult::TOO_LATE;
    if (str == "NO_CARD_ON_TABLE") return MatchResult::NO_CARD_ON_TABLE;
    if (str == "NO_PLAYER_CARD") return MatchResult::NO_PLAYER_CARD;
    if (str == "INVALID_REQUEST") return MatchResult::INVALID_REQUEST;
    return MatchResult::INVALID_REQUEST;
}

} // namespace GameEnums

#endif // GAME_ENUMS_HPP
