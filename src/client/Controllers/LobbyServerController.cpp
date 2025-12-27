#include "LobbyServerController.hpp"

LobbyServerController::LobbyServerController(QObject* parent) {
  Q_UNUSED(parent);
  socket_ = new QTcpSocket(this);
}

void LobbyServerController::onNicknameVerified(bool isAvailable) {
  // TODO: Implement nickname verification logic
  Q_UNUSED(isAvailable);
}

void LobbyServerController::onGameCreated(const QString& gameId) {
  // TODO: Implement game created logic
  Q_UNUSED(gameId);
}

void LobbyServerController::onGameJoined(bool success, const QString& reason) {
  // TODO: Implement game joined logic
  Q_UNUSED(success);
  Q_UNUSED(reason);
}

void LobbyServerController::onGameObserved(bool success, const QString& reason) {
  // TODO: Implement game observed logic
  Q_UNUSED(success);
  Q_UNUSED(reason);
}

void LobbyServerController::onDisconnected() {
  // TODO: Implement disconnection logic
}

void LobbyServerController::onError(QAbstractSocket::SocketError socketError) {
  // TODO: Implement error handling logic
  Q_UNUSED(socketError);
}
