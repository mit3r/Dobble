#include "LobbyServerController.hpp"

void LobbyServerController::wantNicknameVerification(const QString& nickname) {
  qDebug() << "LobbyController: Requesting nickname verification for:" << nickname;
  currentNickname = nickname;  // Store nickname for later use

  SenderLoginCommand cmd;
  cmd.command = "login";

  cmd.data_obj = SenderLoginCommand::data();
  cmd.data_obj->nickname = nickname.toStdString();

  json j = cmd;
  send_json_packet(socket->socketDescriptor(), j);
}

void LobbyServerController::wantCreateGame(const QString& gameName) {
  // TODO: Implement game created logic
  Q_UNUSED(gameName);
}

void LobbyServerController::wantJoinGame(const QString& gameId) {
  // TODO: Implement game joined logic
  Q_UNUSED(gameId);
}

void LobbyServerController::wantObserveGame(const QString& gameId) {
  // TODO: Implement game observed logic
  Q_UNUSED(gameId);
}

void LobbyServerController::wantDisconnect() {
  // TODO: Implement disconnection logic
}
