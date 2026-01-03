#include "InactivityMonitor.hpp"

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
      g_server_running = false;
      exit(0);
    }
  }
}
