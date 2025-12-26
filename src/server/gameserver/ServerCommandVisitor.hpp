#include <protocol/LobbyCommandFactory.hpp>
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
  ServersCommandFactory& factory;

  ServerCommandVisitor(int sock, ServersCommandFactory& f);

  private:
  void sendErrorResponse(
      const std::string& commandName,
      const std::string& code,
      const std::string& message);

  public:
  void operator()(const std::monostate&);

  template <typename T>
  void operator()(const T& cmd);
};

template <typename T>
void ServerCommandVisitor::operator()(const T& cmd) {
  if constexpr (std::is_base_of_v<ServersCommandFactory, T>) {
    std::cout << "[INFO] Otrzymano inna komende: " << cmd.command << std::endl;
  } else {
    std::cout << "[INFO] Otrzymano nieznany typ wariantu." << std::endl;
  }
}
