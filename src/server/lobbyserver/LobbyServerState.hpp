#include "server/common/ServerUtils.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

#pragma once

class LobbyServerState : public ServerStateManager {
  private:
  std::vector<std::shared_ptr<GameServer>> game_servers;
  std::mutex game_servers_mutex;
  std::string uds_path;

  public:
  LobbyServerState();
  void setUDSPath(const std::string& path);
  std::string getUDSPath() const;
};
