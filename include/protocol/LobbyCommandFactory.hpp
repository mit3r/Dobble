#include <functional>
#include <map>
#include <string>

#include "./lobby/lobbyclient/SocketCommands.hpp"

#pragma once

class LobbyCommandFactory {
  private:
  using CreatorFunc = std::function<LobbyClientCommand(const json&)>;
  std::map<std::string, CreatorFunc> dictionary;

  bool isResponse(const json& j) {  // is_server determine its a response command
    return j.contains("lobby_server_id") && !j["lobby_server_id"].is_null();
  }

  public:
  LobbyCommandFactory() {
    dictionary["login"] = [this](const json& j) -> LobbyClientCommand {
      if (isResponse(j))
        return j.get<ResponseLoginCommand>();
      else
        return j.get<SenderLoginCommand>();
    };

    dictionary["ping"] = [this](const json& j) -> LobbyClientCommand {
      if (isResponse(j))
        return j.get<ResponsePingCommand>();
      else
        return j.get<SenderPingCommand>();
    };

    dictionary["getinfolobby"] = [this](const json& j) -> LobbyClientCommand {
      if (isResponse(j))
        return j.get<ResponseGetLobbyInfoCommand>();
      else
        return j.get<SenderGetLobbyInfoCommand>();
    };
    
    dictionary["create_lobby"] = [this](const json& j) -> LobbyClientCommand {
      if (isResponse(j))
        return j.get<ResponseCreateLobbyCommand>();
      else
        return j.get<SenderCreateLobbyCommand>();
    };
  }

  LobbyClientCommand get(const std::string& commandName, const json& j) {
    if (dictionary.count(commandName)) {
      return dictionary[commandName](j);
    }
    return std::monostate{};
  }

  // KISS version
  LobbyClientCommand get(const json& j) {
    if (!j.contains("command")) return std::monostate{};

    std::string commandName = j["command"];
    if (!dictionary.count(commandName)) return std::monostate{};

    return dictionary[commandName](j);
  }
};