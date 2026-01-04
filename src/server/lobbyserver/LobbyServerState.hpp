#include "server/common/ServerUtils.hpp"
#include "server/libraries.hpp"
#include "server/network.hpp"

#pragma once

class LobbyServerState : public ServerStateManager {
  private:
  std::vector<std::shared_ptr<GameServer>> game_servers;
  std::mutex game_servers_mutex;
  std::string uds_path;
  std::string public_ip;

  public:
  LobbyServerState();
  // void setUDSPath(const std::string& path);
  std::string getUDSPath() const;
  void setPublicIP(const std::string& ip);
  std::string getPublicIP() const;
};
