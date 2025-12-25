#include <list>
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <variant>

#pragma once

using json = nlohmann::json;

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
}  // namespace nlohmann

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

class GameServerMessageCore {
  public:
  std::string command;
  std::optional<std::string> lobby_server_id;
  std::optional<std::string> client_id;
  std::optional<std::string> game_id;
  std::optional<ErrorStruct> error;
};

class ErrorResponse : public GameServerMessageCore {
  public:
  struct data {
    std::string message;
  };
  std::optional<data> data_obj;
  std::string code;
  std::string message;
};


class SenderRegisterGameServerCommand : public GameServerMessageCore {
  public:
      struct data {
        std::string ip;
        std::string port;
        std::string max_players;
        std::string game_name;
      };
      std::optional<data> data_obj;
};


class ResponseRegisterGameServerCommand : public GameServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};


class SenderEndGameCommand : public GameServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

class ResponseEndGameCommand : public GameServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

class SenderPingCommand : public GameServerMessageCore {
  public:
      struct data {
        std::string player_number;
        std::string status;
        std::string turn;
        std::string message;
      };
      std::optional<data> data_obj;
};
class ResponsePingCommand : public GameServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

using GameServerCommand = std::variant<
    std::monostate,
    SenderRegisterGameServerCommand,
    ResponseRegisterGameServerCommand,
    SenderEndGameCommand,
    ResponseEndGameCommand,
    SenderPingCommand,
    ResponsePingCommand
    >;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImageInfo, img_name, rotate, rotation, place, size)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerGameInfo, client_id, imgs, score, points, mistakes, rank)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScoreEntry, client_id, score)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PastTurn, turn_id, winner, winner_id, active)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TurnStruct, turn_id, active, clients_data, clients_id_data, images_on_table, scoreboard, past_turns, winner_id, actual_imgs, players_imgs)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameStruct, game_id, game_name, players, max_players, nicknames, status)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShortGameInfo, game_id, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ErrorStruct, code, message)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ErrorResponse::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderEndGameCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseEndGameCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderRegisterGameServerCommand::data, ip, port, max_players, game_name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseRegisterGameServerCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderPingCommand::data, player_number, status, turn)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponsePingCommand::data, message)


#define DEFINE_JSON_WITH_DATA_RENAME(Type)                                                \
  inline void to_json(json& j, const Type& p) {                                         \
    j = json{{"command", p.command}, {"error", p.error}};                                 \
    if (p.lobby_server_id) j["lobby_server_id"] = p.lobby_server_id;                      \
    if (p.client_id) j["client_id"] = p.client_id;                                        \
    if (p.game_id) j["game_id"] = p.game_id;                                              \
    if (p.data_obj)                                                                       \
      j["data"] = p.data_obj;                                                             \
    else                                                                                  \
      j["data"] = nullptr;                                                                \
  }                                                                                       \
  inline void from_json(const json& j, Type& p) {                                         \
    j.at("command").get_to(p.command);                                                    \
    if (j.contains("lobby_server_id")) j.at("lobby_server_id").get_to(p.lobby_server_id); \
    if (j.contains("game_id")) j.at("game_id").get_to(p.game_id);                         \
    if (j.contains("data") && !j["data"].is_null()) j.at("data").get_to(p.data_obj);      \
    if (j.contains("error") && !j["error"].is_null()) j.at("error").get_to(p.error);      \
  }


DEFINE_JSON_WITH_DATA_RENAME(ErrorResponse)
DEFINE_JSON_WITH_DATA_RENAME(SenderRegisterGameServerCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseRegisterGameServerCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderEndGameCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseEndGameCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderPingCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponsePingCommand)
