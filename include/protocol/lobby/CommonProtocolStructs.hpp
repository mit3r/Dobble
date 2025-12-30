#pragma once

#include <list>
#include <vector>
#include <map>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>

using json = nlohmann::json;

#ifndef NLOHMANN_OPTIONAL_SERIALIZER_DEFINED
#define NLOHMANN_OPTIONAL_SERIALIZER_DEFINED
namespace nlohmann {
template <typename T>
struct adl_serializer<std::optional<T>> {
  static void to_json(json& j, const std::optional<T>& opt) {
    if (opt == std::nullopt)
      j = nullptr;
    else
      j = *opt;
  }
  static void from_json(const json& j, std::optional<T>& opt) {
    if (j.is_null())
      opt = std::nullopt;
    else
      opt = j.get<T>();
  }
};
}
#endif

struct PlayerGameInfo {
  std::string nickname;
  int cardId;
  int score;
  int points;
  int mistakes;
  int rank;
};

struct TurnStruct {
  std::string turn_id;
  bool active;
  std::list<PlayerGameInfo> clients_data;
  std::vector<int> images_on_table;
  std::vector<int> past_turns;
  std::string winner_id;
  std::vector<int> actual_imgs;
  int topCardId;
  int cardsLeft;  // 0 - 57
  std::string status;  // 3  states
};

struct GameStruct {
  std::string game_id;
  std::string game_name;
  std::string players;
  std::string max_players;
  std::list<std::string> nicknames;
  std::string status;
};

struct ShortGameInfo {
  std::string game_id;
  std::string name;
};

struct ErrorStruct {
  std::optional<int> code;
  std::string message;
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerGameInfo, nickname, cardId, score, points, mistakes, rank)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TurnStruct, turn_id, active, clients_data, images_on_table, past_turns, winner_id, actual_imgs, topCardId, cardsLeft, status)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameStruct, game_id, game_name, players, max_players, nicknames, status)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShortGameInfo, game_id, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ErrorStruct, code, message)
