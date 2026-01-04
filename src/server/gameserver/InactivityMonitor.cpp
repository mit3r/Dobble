#include "InactivityMonitor.hpp"
#include <protocol/ServersCommandFactory.hpp>
#include <protocol/lobby/lobbyroom/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include <protocol/GameEnums.hpp>

static void send_end_game_to_lobby(const std::string& reason) {
  if (!g_uds_client || g_uds_client->getSocket() == -1) {
    return;
  }
  
  try {
    SenderEndGameCommand end_cmd;
    end_cmd.command = "end_game";
    end_cmd.game_id = g_game_server.getGameId();
    
    SenderEndGameCommand::data msg_data;
    msg_data.message = reason;
    end_cmd.data_obj = msg_data;
    
    json j = end_cmd;
    send_json_packet(g_uds_client->getSocket(), j);
    std::cout << "[MONITOR] Sent end_game command to lobby server: " << reason << std::endl;
    
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
  } catch (const std::exception& e) {
    std::cerr << "[MONITOR] Error sending end_game command: " << e.what() << std::endl;
  }
}

static void shutdown_server(const std::string& reason) {
  std::cout << "[MONITOR] Shutting down server: " << reason << std::endl;
  send_end_game_to_lobby(reason);
  g_server_running = false;
  exit(0);
}

static void handle_game_over_countdown(bool& game_over_detected, 
                                       std::chrono::steady_clock::time_point& game_over_timestamp) {
  if (g_game_server.getGameStatus() != GameEnums::toString(GameEnums::GameStatus::GAME_OVER)) {
    if (game_over_detected) {
      game_over_detected = false;
      std::cout << "[MONITOR] Game status changed from GAME_OVER, reset shutdown timer" << std::endl;
    }
    return;
  }
  
  if (!game_over_detected) {
    game_over_detected = true;
    game_over_timestamp = std::chrono::steady_clock::now();
    std::cout << "[MONITOR] Game is GAME_OVER. Server will shut down in 60 seconds..." << std::endl;
    return;
  }
  
  auto elapsed = std::chrono::duration_cast<std::chrono::seconds>(
    std::chrono::steady_clock::now() - game_over_timestamp
  );
  
  if (elapsed.count() >= 60) {
    shutdown_server("Server shutting down - game finished");
  } else {
    std::cout << "[MONITOR] Game over countdown: " << (60 - elapsed.count()) << " seconds remaining..." << std::endl;
  }
}

static void disconnect_inactive_player(const std::string& client_id) {
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

static void handle_inactive_players(const std::chrono::seconds& timeout) {
  auto inactive_players = g_game_server.getInactivePlayers(timeout);
  
  for (const auto& client_id : inactive_players) {
    disconnect_inactive_player(client_id);
  }
  
  if (g_game_server.getPlayerCount() == 0 && !inactive_players.empty()) {
    shutdown_server("Server shutting down - all players disconnected");
  }
}

void inactivity_monitor_thread() {
  const std::chrono::seconds timeout(60);
  auto game_over_timestamp = std::chrono::steady_clock::time_point();
  bool game_over_detected = false;
  
  while (g_server_running) {
    std::this_thread::sleep_for(std::chrono::seconds(10));
    
    handle_game_over_countdown(game_over_detected, game_over_timestamp);
    handle_inactive_players(timeout);
  }
}
