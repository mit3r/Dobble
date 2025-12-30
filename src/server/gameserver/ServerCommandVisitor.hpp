#include <protocol/LobbyCommandFactory.hpp>
#include <protocol/GameCommandFactory.hpp>
#include <protocol/lobby/lobbyclient/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include <protocol/ServersCommandFactory.hpp>
#include "server/common/ServerUtils.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

#pragma once

using json = nlohmann::json;

struct ServerCommandVisitor {
  int client_sock;
  std::variant<GameCommandFactory, ServersCommandFactory> factory;
  ServerCommandVisitor(int sock, ServersCommandFactory& f);
  ServerCommandVisitor(int sock, GameCommandFactory& f);


  private:
  void sendErrorResponse(
      const std::string& commandName,
      const std::string& code,
      const std::string& message);

  public:
  void operator()(const std::monostate&);
  void operator()(const SenderJoinGameCommand& cmd);
  void operator()(const ResponseGameClientPingCommand& cmd);
  void operator()(const SenderGameClientPingCommand& cmd);
  void operator()(const ResponseJoinGameCommand& cmd);
  void operator()(const SenderLeaveRoomCommand& cmd);
  void operator()(const ResponseLeaveRoomCommand& cmd);
  void operator()(const SenderSendGameInfoCommand& cmd);
  void operator()(const ResponseSendGameInfoCommand& cmd);  

  template <typename T>
  void operator()(const T& cmd);
};

template <typename T>
void ServerCommandVisitor::operator()(const T& cmd) {
  if constexpr (std::is_base_of_v<ServersCommandFactory, T>) {
    std::cout << "[USD] Otrzymano inna komende: " << cmd.command << std::endl;
  } else if constexpr (std::is_base_of_v<GameCommandFactory, T>) {
    std::cout << "[BSD] Otrzymano inna komende: " << cmd.command << std::endl;
  } else {
    std::cout << "[INFO] Otrzymano nieznany typ wariantu." << std::endl;
  }
}
