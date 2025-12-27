#include "LobbyServerController.hpp"

void LobbyServerController::wantNicknameVerification(bool isAvailable) {
  // TODO: Implement nickname verification logic
  Q_UNUSED(isAvailable);
}

void LobbyServerController::wantCreateGame(const QString& gameId) {
  // TODO: Implement game created logic
  Q_UNUSED(gameId);
}

void LobbyServerController::wantJoinGame(bool success, const QString& reason) {
  // TODO: Implement game joined logic
  Q_UNUSED(success);
  Q_UNUSED(reason);
}

void LobbyServerController::wantObserveGame(bool success, const QString& reason) {
  // TODO: Implement game observed logic
  Q_UNUSED(success);
  Q_UNUSED(reason);
}

void LobbyServerController::wantDisconnect() {
  // TODO: Implement disconnection logic
}
