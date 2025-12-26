#pragma once

#include <list>
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

struct ImageInfo {
  std::string img_name;
  std::optional<std::string> rotate;
  std::optional<std::string> rotation;
  std::optional<std::string> place;
  std::optional<std::string> size;
};

struct PlayerGameInfo {
  std::string client_id;
  std::optional<std::list<ImageInfo>> imgs;
  std::optional<int> score;
  std::optional<int> points;
  std::optional<int> mistakes;
  std::optional<int> rank;
};

struct ScoreEntry {
  std::string client_id;
  std::string score;
};

struct PastTurn {
  std::string turn_id;
  std::optional<std::string> winner;
  std::optional<std::string> winner_id;
  std::optional<std::string> active;
};

struct TurnStruct {
  std::string turn_id;
  std::optional<bool> active;
  std::optional<std::list<PlayerGameInfo>> clients_data;
  std::optional<std::list<ImageInfo>> images_on_table;
  std::optional<std::list<ScoreEntry>> scoreboard;
  std::optional<std::list<PastTurn>> past_turns;
  std::optional<std::string> winner_id;
  std::optional<std::list<PlayerGameInfo>> clients_id_data;
  std::optional<std::list<std::string>> actual_imgs;
  std::optional<std::list<PlayerGameInfo>> players_imgs;
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

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImageInfo, img_name, rotate, rotation, place, size)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerGameInfo, client_id, imgs, score, points, mistakes, rank)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScoreEntry, client_id, score)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PastTurn, turn_id, winner, winner_id, active)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TurnStruct, turn_id, active, clients_data, clients_id_data, images_on_table, scoreboard, past_turns, winner_id, actual_imgs, players_imgs)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameStruct, game_id, game_name, players, max_players, nicknames, status)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShortGameInfo, game_id, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ErrorStruct, code, message)
