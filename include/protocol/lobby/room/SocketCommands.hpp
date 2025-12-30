
#include <nlohmann/json.hpp>
#include <optional>
#include <string>
#include <variant>

#pragma once
#include <protocol/lobby/CommonProtocolStructs.hpp>

using json = nlohmann::json;

struct ConfigData {
    std::string param1;
    int param2;
};

class GameClientMessageCore {
  public:
  std::string command;
  std::optional<std::string> lobby_server_id;
  std::optional<std::string> client_id;
  std::optional<std::string> game_id;
  std::optional<ErrorStruct> error;
};

struct RoomServerReceivedMessage {
    std::string command;
    std::string server_id;
    ConfigData data;               
    std::optional<std::string> error; 
};

struct RoomClientReceivedMessage {
    std::string command;
    std::string client_id;
    ConfigData data;               
    std::optional<std::string> error; 
};

class ResponseGameClientPingCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};


class SenderGameClientPingCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

class SenderJoinGameCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string game_id;
        std::string role;
      };
      std::optional<data> data_obj;
};

class ResponseJoinGameCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string status;
        std::string role;
        ShortGameInfo game_info;
      };
      std::optional<data> data_obj;
};

class SenderLeaveRoomCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string game_id;
      };
      std::optional<data> data_obj;
};

class ResponseLeaveRoomCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string message;
      };
      std::optional<data> data_obj;
};

class SenderSendGameInfoCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string game_id;
      };
      std::optional<data> data_obj;
};

class ResponseSendGameInfoCommand : public GameClientMessageCore {
  public:
      struct data {
        std::string game_id;
        TurnStruct actual_turn;
      };
      std::optional<data> data_obj;
};

using GameClientCommand = std::variant<
    std::monostate,
    RoomServerReceivedMessage,
    RoomClientReceivedMessage,
    ResponseGameClientPingCommand,
    SenderGameClientPingCommand,
    SenderJoinGameCommand,
    ResponseJoinGameCommand,
    SenderLeaveRoomCommand,
    ResponseLeaveRoomCommand,
    SenderSendGameInfoCommand,
    ResponseSendGameInfoCommand
>;


NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ConfigData, param1, param2)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoomServerReceivedMessage, command, server_id, data, error)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(RoomClientReceivedMessage, command, client_id, data, error)

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseGameClientPingCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderGameClientPingCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderJoinGameCommand::data, game_id, role)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseJoinGameCommand::data, status, role, game_info)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderLeaveRoomCommand::data, game_id)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseLeaveRoomCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderSendGameInfoCommand::data, game_id)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseSendGameInfoCommand::data, game_id, actual_turn)

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

DEFINE_JSON_WITH_DATA_RENAME(ResponseGameClientPingCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderGameClientPingCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderJoinGameCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseJoinGameCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderLeaveRoomCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseLeaveRoomCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderSendGameInfoCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseSendGameInfoCommand)
