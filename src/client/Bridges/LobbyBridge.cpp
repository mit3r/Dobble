#include "LobbyBridge.hpp"

void LobbyBridge::setReadyState(bool isReady) {
  qDebug() << "Setting ready state to:" << isReady;
}

void LobbyBridge::quitLobby() {
  qDebug() << "Quitting lobby.";
}
