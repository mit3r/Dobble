#include <protocol/LobbyCommandFactory.hpp>
#include <protocol/ServersCommandFactory.hpp>
#include <protocol/lobby/lobbyclient/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include "server/common/BaseServer.hpp"
#include <algorithm>
#include <iterator>
#include <variant>
#include "LobbyServerState.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

#pragma once

struct ServerCommandVisitor {
    int client_sock;
    std::variant<LobbyCommandFactory, ServersCommandFactory> factory;
    
    ServerCommandVisitor(int sock, LobbyCommandFactory& f);
    
    ServerCommandVisitor(int sock, ServersCommandFactory& f);

private:
    void sendErrorResponse(
        const std::string& commandName, 
        const std::string& code, 
        const std::string& message);

public:
    void operator()(const SenderLoginCommand& cmd);
    void operator()(const SenderPingCommand& cmd);
    void operator()(const SenderGetLobbyInfoCommand& cmd);
    void operator()(const SenderCreateLobbyCommand& cmd);
    void operator()(const SenderRegisterGameServerCommand& cmd);
    void operator()(const ResponseRegisterGameServerCommand& cmd);
    
    void operator()(const std::monostate&);
    
    template <typename T>
    void operator()(const T& cmd);
};

template <typename T>
void ServerCommandVisitor::operator()(const T& cmd) {
    if constexpr (std::is_base_of_v<LobbyServerMessageCore, T>) {
         std::cout << "[INFO] Otrzymano inna komende: " << cmd.command << std::endl;
    } else if constexpr (std::is_base_of_v<GameServerMessageCore, T>) {
            std::cout << "[INFO] Otrzymano inna komende: " << cmd.command << std::endl;
    } else {
         std::cout << "[INFO] Otrzymano nieznany typ wariantu." << std::endl;
    }
}
