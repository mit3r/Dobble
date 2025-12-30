

#include <protocol/LobbyCommandFactory.hpp>
#include <protocol/ServersCommandFactory.hpp>
#include <protocol/GameCommandFactory.hpp>

#include <protocol/lobby/lobbyclient/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include <protocol/lobby/room/SocketCommands.hpp>
#include "ServerUtils.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

#pragma once

class LobbyCommandFactory;

template <typename StateManager, typename VisitorCreator, typename CommandFactory>
void server_handle_client(int client_sock, StateManager& server_state, VisitorCreator create_visitor, CommandFactory& command_factory) {
  std::shared_ptr<Client> me = std::make_shared<Client>();
  me->socket = client_sock;
  me->logged_in = false;

  server_state.addClient(me);
  std::cout << "[SERVER] New client connected: " << client_sock << std::endl;
  
  handle_communication(client_sock, create_visitor, command_factory);
  
  std::cout << "[SERVER] Client disconnected: " << client_sock << std::endl;
  close(client_sock);

  server_state.removeClient(me);
}

template <typename StateManager, typename VisitorCreator, typename CommandFactory>
void client_handle_client(int client_sock, StateManager& server_state, VisitorCreator create_visitor, CommandFactory& command_factory) {
  std::cout << "[CLIENT] Connected to: " << client_sock << std::endl;
  handle_communication(client_sock, create_visitor, command_factory);
  std::cout << "[CLIENT] Client disconnected: " << client_sock << std::endl;
  close(client_sock);
}

template <typename VisitorCreator, typename CommandFactory>
void handle_communication(int sock, VisitorCreator create_visitor, CommandFactory& command_factory) {

  while (true) {
    std::optional<json> msg_opt = receive_json_packet(sock);

    if (!msg_opt.has_value()) {
      continue;  //???
    }

    json msg = msg_opt.value();
    if (msg.contains("command")) {
      std::string cmd_name = msg["command"];
      std::cout << "[SERVER] Received command: " << cmd_name << std::endl;
      auto command_variant = command_factory.get(cmd_name, msg);

      auto visitor = create_visitor(sock, command_factory);
      std::visit(visitor, command_variant);
    } else {
      std::cerr << "[WARNING] Otrzymano JSON bez pola 'command'" << std::endl;
    }
  }


}