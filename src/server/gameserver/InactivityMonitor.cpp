#include "InactivityMonitor.hpp"
#include <protocol/ServersCommandFactory.hpp>
#include <protocol/lobby/lobbyroom/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>

void inactivity_monitor_thread() {
  const std::chrono::seconds timeout(60);
  
  while (g_server_running) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    auto inactive_players = g_game_server.getInactivePlayers(timeout);
    
    for (const auto& client_id : inactive_players) {
      std::cout << "[MONITOR] Player " << client_id << " inactive for more than 60 seconds. Disconnecting..." << std::endl;
      
      try {
        auto& player = g_game_server.getPlayer(client_id);
        if (player.socket_fd != -1) {
          close(player.socket_fd);
        }
      } catch (const std::exception& e) {
        std::cerr << "[MONITOR] Error getting player info: " << e.what() << std::endl;
      }
      
      g_game_server.removePlayer(client_id);
      std::cout << "[MONITOR] Player " << client_id << " removed. Remaining players: " 
                << g_game_server.getPlayerCount() << std::endl;
    }
    
    if (g_game_server.getPlayerCount() == 0 && !inactive_players.empty()) {
      std::cout << "[MONITOR] All players disconnected due to inactivity. Shutting down..." << std::endl;
      
      if (g_uds_client && g_uds_client->getSocket() != -1) {
        try {
          SenderEndGameCommand end_cmd;
          end_cmd.command = "end_game";
          end_cmd.game_id = g_game_server.getGameId();
          
          SenderEndGameCommand::data msg_data;
          msg_data.message = "Server shutting down - all players disconnected";
          end_cmd.data_obj = msg_data;
          
          json j = end_cmd;
          std::string message = j.dump();
          
          send(g_uds_client->getSocket(), message.c_str(), message.length(), 0);
          std::cout << "[MONITOR] Sent end_game command to lobby server" << std::endl;
          
          // Give lobby server time to process
          std::this_thread::sleep_for(std::chrono::milliseconds(500));
        } catch (const std::exception& e) {
          std::cerr << "[MONITOR] Error sending end_game command: " << e.what() << std::endl;
        }
      }
      
      g_server_running = false;
      exit(0);
    }
  }
}
