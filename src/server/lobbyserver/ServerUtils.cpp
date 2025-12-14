#include "ServerUtils.hpp"
#include <algorithm>

std::shared_ptr<Client> findClientBySocket(ServerState& state, int targetSocket) {
    std::lock_guard<std::mutex> lock(state.clients_mutex);

    auto it = std::find_if(state.clients.begin(), state.clients.end(),
                           [targetSocket](const std::shared_ptr<Client>& c) {
                               return c->socket == targetSocket;
                           });

    if (it != state.clients.end()) {
        return *it;
    }

    return nullptr;
}

std::shared_ptr<Client> findClientByNickname(ServerState& state, const std::string& targetNickname) {
    std::lock_guard<std::mutex> lock(state.clients_mutex);

    auto it = std::find_if(state.clients.begin(), state.clients.end(),
                           [targetNickname](const std::shared_ptr<Client>& c) {
                               return c->nickname == targetNickname;
                           });

    if (it != state.clients.end()) {
        return *it;
    }

    return nullptr;
}
