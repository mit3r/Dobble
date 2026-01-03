#include "GameStatusManager.hpp"
#include "GameStateManager.hpp"
#include "server/common/BaseClient.hpp"
#include <protocol/lobby/lobbyroom/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include <iostream>

extern GameStateManager g_game_server;
extern UdsClient* g_uds_client;

void send_status_update_to_lobby() {
  if (!g_uds_client || g_uds_client->getSocket() == -1) {
    std::cerr << "[STATUS] UDS client not connected, cannot send status update" << std::endl;
    return;
  }

  try {
    SenderGameServerStatusCommand status_cmd;
    status_cmd.command = "game_server_status";
    status_cmd.game_id = g_game_server.getGameId();
    
    SenderGameServerStatusCommand::data status_data;
    status_data.player_count = std::to_string(g_game_server.getPlayerCount());
    status_data.max_players = std::to_string(g_game_server.getMaxPlayers());
    status_data.status = g_game_server.getGameStatus();
    status_cmd.data_obj = status_data;
    
    json j = status_cmd;
    send_json_packet(g_uds_client->getSocket(), j);
    
    std::cout << "[STATUS] Sent status update to lobby: Players " 
              << status_data.player_count << "/" << status_data.max_players 
              << ", Status: " << status_data.status << std::endl;
  } catch (const std::exception& e) {
    std::cerr << "[STATUS] Error sending status update: " << e.what() << std::endl;
  }
}
