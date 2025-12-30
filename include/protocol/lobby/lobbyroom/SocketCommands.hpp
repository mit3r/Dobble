#pragma once

#include <list>
#include <optional>
#include <string>
#include <variant>
#include <protocol/lobby/CommonProtocolStructs.hpp>
#include <protocol/json.hpp>

class GameServerMessageCore {
  public:
  std::string command;
  std::optional<std::string> lobby_server_id;
  std::optional<std::string> client_id;
  std::optional<std::string> game_id;
  std::optional<ErrorStruct> error;
};

class GameServerErrorResponse : public GameServerMessageCore {
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

class SenderServersPingCommand : public GameServerMessageCore {
  public:
      struct data {
        std::string player_number;
        std::string status;
        std::string turn;
        std::string message;
      };
      std::optional<data> data_obj;
};
class ResponseServersPingCommand : public GameServerMessageCore {
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
    SenderServersPingCommand,
    ResponseServersPingCommand,
    GameServerErrorResponse
    >;

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(GameServerErrorResponse::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderEndGameCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseEndGameCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderRegisterGameServerCommand::data, ip, port, max_players, game_name)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseRegisterGameServerCommand::data, message)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(SenderServersPingCommand::data, player_number, status, turn)
NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(ResponseServersPingCommand::data, message)
DEFINE_JSON_WITH_DATA_RENAME(GameServerErrorResponse)
DEFINE_JSON_WITH_DATA_RENAME(SenderRegisterGameServerCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseRegisterGameServerCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderEndGameCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseEndGameCommand)
DEFINE_JSON_WITH_DATA_RENAME(SenderServersPingCommand)
DEFINE_JSON_WITH_DATA_RENAME(ResponseServersPingCommand)
