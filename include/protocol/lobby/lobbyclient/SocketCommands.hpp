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

class LobbyServerMessageCore {
  public:
  std::string command;
  std::optional<std::string> lobby_server_id;
  std::optional<std::string> client_id;
  std::optional<std::string> game_id;
  std::optional<ErrorStruct> error;
};

class ErrorResponse : public LobbyServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
      std::string code;
      std::string message;
};

class SenderLoginCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string nickname;
      };
      std::optional<data> data_obj;
};

class ResponseLoginCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

class SenderPingCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};
class ResponsePingCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

class SenderGetLobbyInfoCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string page;
      };
      std::optional<data> data_obj;
};
class ResponseGetLobbyInfoCommand : public LobbyServerMessageCore {
  public:
  struct data {
    std::string page;
    std::list<GameStruct> actual_games;

    std::optional<std::string> next_page;
  };
  std::optional<data> data_obj;
};

class SenderJoinGameCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string game_id;
        std::string role;
      };
      std::optional<data> data_obj;
};
class ResponseJoinGameCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string status;
        std::string role;
        ShortGameInfo game_info;
      };
      std::optional<data> data_obj;
};

class SenderLeaveRoomCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string game_id;
      };
      std::optional<data> data_obj;
};
class ResponseLeaveRoomCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

class SenderSendGameInfoCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string game_id;
      };
      std::optional<data> data_obj;
};
class ResponseSendGameInfoCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string game_id;
        TurnStruct actual_turn;
      };
      std::optional<data> data_obj;
};

class SenderCreateLobbyCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string game_name;
      };
      std::optional<data> data_obj;
};

class ResponseCreateLobbyCommand : public LobbyServerMessageCore {
  public:
      struct data {
        std::string message;
        std::string ip;
        std::string port;
      };

      std::optional<data> data_obj;
};

using LobbyClientCommand = std::variant<
    std::monostate,
    SenderLoginCommand, ResponseLoginCommand,
    SenderPingCommand, ResponsePingCommand,
    SenderGetLobbyInfoCommand, ResponseGetLobbyInfoCommand,
    SenderJoinGameCommand, ResponseJoinGameCommand,
    SenderLeaveRoomCommand, ResponseLeaveRoomCommand,
    SenderSendGameInfoCommand, ResponseSendGameInfoCommand,
    SenderCreateLobbyCommand, ResponseCreateLobbyCommand>;


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ImageInfo, img_name, rotate, rotation, place, size)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PlayerGameInfo, client_id, imgs, score, points, mistakes, rank)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ScoreEntry, client_id, score)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(PastTurn, turn_id, winner, winner_id, active)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(TurnStruct, turn_id, active, clients_data, clients_id_data, images_on_table, scoreboard, past_turns, winner_id, actual_imgs, players_imgs)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameStruct, game_id, game_name, players, max_players, nicknames, status)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ShortGameInfo, game_id, name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ErrorStruct, code, message)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ErrorResponse::data, message)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderLoginCommand::data, nickname)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseLoginCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderPingCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponsePingCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderGetLobbyInfoCommand::data, page)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseGetLobbyInfoCommand::data, page, actual_games, next_page)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderJoinGameCommand::data, game_id, role)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseJoinGameCommand::data, status, role, game_info)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderLeaveRoomCommand::data, game_id)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseLeaveRoomCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderSendGameInfoCommand::data, game_id)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseSendGameInfoCommand::data, game_id, actual_turn)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderCreateLobbyCommand::data, game_name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseCreateLobbyCommand::data, message, ip, port)

#define DEFINE_JSON_WITH_DATA_RENAME(Type)                                                \
  inline void to_json(json& j, const Type& p) {                                           \
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
    if (j.contains("client_id")) j.at("client_id").get_to(p.client_id);                   \
    if (j.contains("game_id")) j.at("game_id").get_to(p.game_id);                         \
    if (j.contains("data") && !j["data"].is_null()) j.at("data").get_to(p.data_obj);      \
    if (j.contains("error") && !j["error"].is_null()) j.at("error").get_to(p.error);      \
  }

DEFINE_JSON_WITH_DATA_RENAME(ErrorResponse)

DEFINE_JSON_WITH_DATA_RENAME(SenderLoginCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseLoginCommand)

DEFINE_JSON_WITH_DATA_RENAME(SenderPingCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponsePingCommand)

DEFINE_JSON_WITH_DATA_RENAME(SenderGetLobbyInfoCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseGetLobbyInfoCommand)

DEFINE_JSON_WITH_DATA_RENAME(SenderJoinGameCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseJoinGameCommand)

DEFINE_JSON_WITH_DATA_RENAME(SenderLeaveRoomCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseLeaveRoomCommand)

DEFINE_JSON_WITH_DATA_RENAME(SenderSendGameInfoCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseSendGameInfoCommand)

DEFINE_JSON_WITH_DATA_RENAME(SenderCreateLobbyCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseCreateLobbyCommand)
