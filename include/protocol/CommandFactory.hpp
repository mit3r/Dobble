#pragma once
#include <map>
#include <functional>
#include <string>
#include "lobby/lobbyclient/SocketCommands.hpp"

class CommandFactory {
private:
    using CreatorFunc = std::function<AnyCommand(const json&)>;
    std::map<std::string, CreatorFunc> dictionary;

    bool isResponse(const json& j) { //is_server determine its a response command
        return j.contains("lobby_server_id") && !j["lobby_server_id"].is_null();
    }

public:
    CommandFactory() {


        dictionary["login"] = [this](const json& j) -> AnyCommand {
            if (isResponse(j)) return j.get<ResponseLoginCommand>();
            else               return j.get<SenderLoginCommand>();
        };
        
        dictionary["ping"] = [this](const json& j) -> AnyCommand {
            if (isResponse(j)) return j.get<ResponsePingCommand>();
            else               return j.get<SenderPingCommand>();
        };

        dictionary["join_game"] = [this](const json& j) -> AnyCommand {
            if (isResponse(j)) return j.get<ResponseJoinGameCommand>();
            else               return j.get<SenderJoinGameCommand>();
        };

        dictionary["getinfolobby"] = [this](const json& j) -> AnyCommand {
            if (isResponse(j)) return j.get<ResponseGetLobbyInfoCommand>();
            else               return j.get<SenderGetLobbyInfoCommand>();
        };

        dictionary["leaveroom"] = [this](const json& j) -> AnyCommand {
            if (isResponse(j)) return j.get<ResponseLeaveRoomCommand>();
            else               return j.get<SenderLeaveRoomCommand>();
        };

        dictionary["sendgame_info"] = [this](const json& j) -> AnyCommand {
            if (isResponse(j)) return j.get<ResponseSendGameInfoCommand>();
            else               return j.get<SenderSendGameInfoCommand>();
        };

        dictionary["create_lobby"] = [this](const json& j) -> AnyCommand {
            if (isResponse(j)) return j.get<ResponseCreateLobbyCommand>();
            else               return j.get<SenderCreateLobbyCommand>();
        };
        
    }

    AnyCommand get(const std::string& commandName, const json& j) {
        if (dictionary.count(commandName)) {
            return dictionary[commandName](j);
        }
        return std::monostate{};
    }
};