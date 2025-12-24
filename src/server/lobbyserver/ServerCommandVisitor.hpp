#include <protocol/CommandFactory.hpp>
#include <protocol/lobby/lobbyclient/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include "server/common/BaseServer.hpp"
#include <algorithm>
#include <iterator>
#include "LobbyServerState.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

#pragma once

using json = nlohmann::json;

struct ServerCommandVisitor {
    int client_sock;        
    CommandFactory& factory;
    
    ServerCommandVisitor(int sock, CommandFactory& f);

private:
    void sendErrorResponse(
        const std::string& commandName, 
        const std::string& code, 
        const std::string& message);

public:
    void operator()(const SenderLoginCommand& cmd);
    void operator()(const SenderPingCommand& cmd);
    void operator()(const SenderJoinGameCommand& cmd);
    void operator()(const SenderGetLobbyInfoCommand& cmd);
    void operator()(const SenderCreateLobbyCommand& cmd);
    void operator()(const std::monostate&);
    
    template <typename T>
    void operator()(const T& cmd);
};

template <typename T>
void ServerCommandVisitor::operator()(const T& cmd) {
    if constexpr (std::is_base_of_v<LobbyServerMessageCore, T>) {
         std::cout << "[INFO] Otrzymano inna komende: " << cmd.command << std::endl;
    } else {
         std::cout << "[INFO] Otrzymano nieznany typ wariantu." << std::endl;
    }
}
