#include <functional>
#include <map>
#include <string>

#include "protocol/lobby/room/SocketCommands.hpp"

#pragma once

class GameCommandFactory {
  private:
  using CreatorFunc = std::function<GameClientCommand(const json&)>;
  std::map<std::string, CreatorFunc> dictionary;

  bool isResponse(const json& j) {  // is_server determine its a response command
    return j.contains("game_server_id") && !j["game_server_id"].is_null();
  }

  public:
  GameCommandFactory() {
    dictionary["ping"] = [this](const json& j) -> GameClientCommand {
      if (isResponse(j))
        return j.get<ResponseGameClientPingCommand>();
      else
        return j.get<SenderGameClientPingCommand>();
    };

    dictionary["join_game"] = [this](const json& j) -> GameClientCommand {
      if (isResponse(j))
        return j.get<ResponseJoinGameCommand>();
      else
        return j.get<SenderJoinGameCommand>();
    };

    dictionary["leave_room"] = [this](const json& j) -> GameClientCommand {
      if (isResponse(j))
        return j.get<ResponseLeaveRoomCommand>();
      else
        return j.get<SenderLeaveRoomCommand>();
    };

    dictionary["send_game_info"] = [this](const json& j) -> GameClientCommand {
      if (isResponse(j))
        return j.get<ResponseSendGameInfoCommand>();
      else
        return j.get<SenderSendGameInfoCommand>();
    };

    dictionary["start_game"] = [this](const json& j) -> GameClientCommand {
      if (isResponse(j))
        return j.get<ResponseStartGameCommand>();
      else
        return j.get<SenderStartGameCommand>();
    };

    dictionary["match_symbol"] = [this](const json& j) -> GameClientCommand {
      if (isResponse(j))
        return j.get<ResponseMatchSymbolCommand>();
      else
        return j.get<SenderMatchSymbolCommand>();
    };

  }

  GameClientCommand get(const std::string& commandName, const json& j) {
    if (dictionary.count(commandName)) {
      return dictionary[commandName](j);
    }
    return std::monostate{};
  }
};