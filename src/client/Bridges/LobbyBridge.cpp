#include "LobbyBridge.hpp"

void LobbyBridge::callSetReadyState(bool isReady) {
  qDebug() << "LobbyBridge: callSetReadyState called with isReady =" << isReady;
  emit requestSetReadyState(isReady);
}

void LobbyBridge::callQuitLobby() {
  qDebug() << "LobbyBridge: callQuitLobby called";
  emit onLobbyQuit();
}

void LobbyBridge::hasLobbyNameChanged(const QString& lobbyName) {
  qDebug() << "LobbyBridge: hasLobbyNameChanged received with lobbyName =" << lobbyName;
  emit onLobbyNameChanged(lobbyName);
}

void LobbyBridge::hasPlayerListChanged(const QStringList& playerList) {
  qDebug() << "LobbyBridge: hasPlayerListChanged received with playerList =" << playerList;
  emit onPlayersListChanged(playerList);
}

void LobbyBridge::hasLobbyStatusChanged(const GameStatus& status) {
  qDebug() << "LobbyBridge: hasLobbyStatusChanged received with status =" << static_cast<int>(status);
  emit onLobbyStatusChanged(status);
}

void LobbyBridge::hasSetReadyState(bool isReady) {
  qDebug() << "LobbyBridge: hasSetReadyState received with isReady =" << isReady;
  emit onReadyStateChanged(isReady);
}

void LobbyBridge::hasQuitLobby() {
  qDebug() << "LobbyBridge: hasQuitLobby received";
  emit onLobbyQuit();
}
