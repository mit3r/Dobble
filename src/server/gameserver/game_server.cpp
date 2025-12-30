#include "server/common/BaseServer.hpp"
#include "server/common/ServerUtils.hpp"
#include "server/gameserver/GameClientHandler.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"
#include "server/common/BaseClient.hpp"
#include "GameStateManager.hpp"

GameStateManager g_game_server;

int main(int argc, char* argv[]) {
  if (argc < 4) {
    std::cerr << "Usage: " << argv[0] << " <server_id> <port> <lobby_uds_path>" << std::endl;
    return 1;
  }

  std::string server_id = argv[1];
  int port = std::stoi(argv[2]);
  std::string lobby_uds_path =  "/tmp/lobby_uds_socket";
  g_game_server.server_id = server_id;
  g_game_server.port = port;
  g_game_server.ip = "0.0.0.0";
  g_game_server.max_players = "4";
  g_game_server.name = "Dobble Game Server";

  std::cout << "[GameServer] Starting with ID: " << server_id
            << ", Port: " << port
            << ", Lobby UDS: " << lobby_uds_path << std::endl;

  BsdServer bsd_server(port, "127.0.0.1");
  UdsClient uds_client(lobby_uds_path);

  bsd_server.setClientHandler(tcp_client_handler);
  uds_client.setClientHandler(uds_client_handler);
  
  std::thread bsd_thread([&bsd_server]() {
    bsd_server.run();
  });


  std::thread uds_thread([&uds_client]() {
    uds_client.connect();
  });

  

  // bsd_thread.join();
  uds_thread.join();

  return 0;
}