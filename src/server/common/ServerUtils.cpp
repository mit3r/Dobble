#include "ServerUtils.hpp"

ServerStateManager::ServerStateManager() {}

ServerStateManager::~ServerStateManager() {}

void ServerStateManager::addClient(std::shared_ptr<Client> client) {
  std::lock_guard<std::mutex> lock(clients_mutex);
  clients.push_back(client);
}

void ServerStateManager::removeClient(std::shared_ptr<Client> client) {
  std::lock_guard<std::mutex> lock(clients_mutex);
  auto it = std::remove(clients.begin(), clients.end(), client);
  clients.erase(it, clients.end());
}

std::shared_ptr<Client> ServerStateManager::findClientBySocket(int targetSocket) {
  std::lock_guard<std::mutex> lock(clients_mutex);

  auto it = std::find_if(clients.begin(), clients.end(),
                         [targetSocket](const std::shared_ptr<Client>& c) {
    return c->socket == targetSocket;
  });

  if (it != clients.end()) {
    return *it;
  }

  return nullptr;
}

std::shared_ptr<Client> ServerStateManager::findClientByNickname(const std::string& targetNickname) {
  std::lock_guard<std::mutex> lock(clients_mutex);

  auto it = std::find_if(clients.begin(), clients.end(),
                         [targetNickname](const std::shared_ptr<Client>& c) {
    return c->nickname == targetNickname;
  });

  if (it != clients.end()) {
    return *it;
  }

  return nullptr;
}

size_t ServerStateManager::getClientCount() const {
  return clients.size();
}

std::vector<std::shared_ptr<Client>> ServerStateManager::getClients() {
  std::lock_guard<std::mutex> lock(clients_mutex);
  return clients;
}

int find_available_port() {
  static int last_port = 2000;
  return ++last_port;
}
