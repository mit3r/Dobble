

#include <protocol/LobbyCommandFactory.hpp>
#include <protocol/ServersCommandFactory.hpp>

#include <protocol/lobby/lobbyclient/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>

#include "ServerUtils.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

#pragma once

class LobbyCommandFactory;

using json = nlohmann::json;

template <typename StateManager, typename VisitorCreator>
void server_handle_client(int client_sock, StateManager& server_state, VisitorCreator create_visitor) {
  std::shared_ptr<Client> me = std::make_shared<Client>();
  me->socket = client_sock;
  me->logged_in = false;

  server_state.addClient(me);
  std::cout << "[SERVER] New client connected: " << client_sock << std::endl;
  handle_communication(client_sock, create_visitor);
  std::cout << "[SERVER] Client disconnected: " << client_sock << std::endl;
  close(client_sock);

  server_state.removeClient(me);
}

template <typename ClientManager, typename VisitorCreator>
void client_handle_client(int server_sock, ClientManager& client_manager, VisitorCreator create_visitor) {
  std::cout << "[CLIENT] Connected to: " << server_sock << std::endl;
  handle_communication(server_sock, create_visitor);
  std::cout << "[CLIENT] Client disconnected: " << server_sock << std::endl;
  close(server_sock);
}

template <typename VisitorCreator>
void handle_communication(int sock, VisitorCreator create_visitor) {

  while (true) {
    std::optional<json> msg_opt = receive_json_packet(sock);

    if (!msg_opt.has_value()) {
      break;  //???
    }

    json msg = msg_opt.value();
    if (msg.contains("command")) {
      std::string cmd_name = msg["command"];
      auto command_variant = create_visitor.get(cmd_name, msg);

      auto visitor = create_visitor(sock, command_factory);
      std::visit(visitor, command_variant);
    } else {
      std::cerr << "[WARNING] Otrzymano JSON bez pola 'command'" << std::endl;
    }
  }


}