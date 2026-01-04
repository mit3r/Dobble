#include "LobbyServerState.hpp"

LobbyServerState::LobbyServerState() : ServerStateManager() {}

// void LobbyServerState::setUDSPath(const std::string& path) {
//     uds_path = path;
// }

std::string LobbyServerState::getUDSPath() const {
    return uds_path;
}

void LobbyServerState::setPublicIP(const std::string& ip) {
    public_ip = ip;
}

std::string LobbyServerState::getPublicIP() const {
    return public_ip;
}
