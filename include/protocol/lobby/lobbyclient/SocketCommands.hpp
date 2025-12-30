#pragma once

#include <list>
#include <optional>
#include <string>
#include <variant>
#include <protocol/lobby/CommonProtocolStructs.hpp>
#include <protocol/json.hpp>

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
    SenderCreateLobbyCommand, ResponseCreateLobbyCommand>;

// Serializacja JSON dla klas specyficznych dla lobby client
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ErrorResponse::data, message)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderLoginCommand::data, nickname)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseLoginCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderPingCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponsePingCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderGetLobbyInfoCommand::data, page)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseGetLobbyInfoCommand::data, page, actual_games, next_page)
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

DEFINE_JSON_WITH_DATA_RENAME(SenderCreateLobbyCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseCreateLobbyCommand)
