#include <functional>
#include <map>
#include <string>

#include "./lobby/lobbyroom/SocketCommands.hpp"

#pragma once

class ServersCommandFactory {
  private:
  using CreatorFunc = std::function<GameServerCommand(const json&)>;
  std::map<std::string, CreatorFunc> dictionary;

  bool isResponse(const json& j) {  // is_server determine its a response command
    return j.contains("lobby_server_id") && !j["lobby_server_id"].is_null();
  }

  public:
  ServersCommandFactory() {
    dictionary["register_game_server"] = [this](const json& j) -> GameServerCommand {
      if (isResponse(j))
        return j.get<ResponseRegisterGameServerCommand>();
      else
        return j.get<SenderRegisterGameServerCommand>();
    };

    dictionary["end_game"] = [this](const json& j) -> GameServerCommand {
      if (isResponse(j))
        return j.get<ResponseEndGameCommand>();
      else
        return j.get<SenderEndGameCommand>();
    };
    dictionary["ping"] = [this](const json& j) -> GameServerCommand {
      if (isResponse(j))
        return j.get<ResponsePingCommand>();
      else
        return j.get<SenderPingCommand>();
    };
  }

  GameServerCommand get(const std::string& commandName, const json& j) {
    if (dictionary.count(commandName)) {
      return dictionary[commandName](j);
    }
    return std::monostate{};
  }
};