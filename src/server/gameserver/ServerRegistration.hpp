#pragma once

#include <protocol/lobby/lobbyroom/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include "server/libraries.hpp"
#include "server/network.hpp"
#include "GameStateManager.hpp"

extern GameStateManager g_game_server;

template <typename StateManager>
void registerServer(StateManager& server_state, int client_sock) {

  SenderRegisterGameServerCommand register_cmd;
  register_cmd.command = "register_game_server";
  register_cmd.lobby_server_id = g_game_server.server_id;
  register_cmd.game_id = g_game_server.server_id;
  // register_cmd.game_id = gen_random(16);

  
  SenderRegisterGameServerCommand::data data;
  data.ip = std::string(g_game_server.ip);
  data.port = std::to_string(g_game_server.port);
  data.max_players = std::string(g_game_server.max_players);
  data.game_name = g_game_server.name;

  register_cmd.data_obj = data;

  std::cout << "[UDS] Registering to the lobby server on socket: " << client_sock << std::endl;
  
  json j = register_cmd;
  bool success = send_json_packet(client_sock, j);
  

  if (success) {
    std::cout << "[UDS] Registration request sent successfully!" << std::endl;
  } else {
    std::cerr << "[UDS] Failed to send registration request!" << std::endl;
  }
  json response = receive_json_packet(client_sock); // wait for response
  

}
