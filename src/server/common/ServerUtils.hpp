
#include <sys/types.h>

#include <algorithm>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#pragma once

struct Client {
  int socket;
  std::string nickname;
  std::string client_id;
  std::string game_id;
  bool logged_in;
};

struct GameServer {
  std::string servername;
  int max_players;
  int players;
  int server_socket;
  int socket;
  std::string server_id;
  
  bool registered;
  int port;
  int process_pid;
  std::string ip; 
};

class ServerStateManager {
  protected:
  std::vector<std::shared_ptr<Client>> clients;
  std::mutex clients_mutex;

  public:
  std::string server_id;
  int port;
  std::string ip;
  std::string max_players;
  std::string name;

  struct GameInfo {
    std::string game_name;
    std::string max_players;
    std::string players;
  };

  ServerStateManager();
  virtual ~ServerStateManager();
  virtual void addClient(std::shared_ptr<Client> client);
  virtual void removeClient(std::shared_ptr<Client> client);
  virtual std::shared_ptr<Client> findClientBySocket(int targetSocket);
  virtual std::shared_ptr<Client> findClientByNickname(const std::string& targetNickname);
  size_t getClientCount() const;
  std::vector<std::shared_ptr<Client>> getClients();
};

int find_available_port();
