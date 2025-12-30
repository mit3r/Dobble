#include "LobbyServerController.hpp"

void LobbyServerController::wantNicknameVerification(const std::string& nickname) {
  qDebug() << "LobbyController: Requesting nickname verification for:"
           << QString::fromStdString(nickname);
  this->nickname = nickname; // Store nickname for later use

  SenderLoginCommand cmd;
  cmd.command = "login";

  cmd.data_obj = SenderLoginCommand::data();
  cmd.data_obj->nickname = nickname;

  json j = cmd;
  send_json_packet(socket->socketDescriptor(), json(cmd));
}

void LobbyServerController::wantCreateGame(const std::string& gameName) {
  qDebug() << "LobbyController: Requesting game creation with name:"
           << QString::fromStdString(gameName);

  SenderCreateLobbyCommand cmd;
  cmd.command = "create_lobby";
  cmd.data_obj = SenderCreateLobbyCommand::data();
  cmd.data_obj->game_name = gameName;

  send_json_packet(socket->socketDescriptor(), json(cmd));
}

void LobbyServerController::wantJoinGame(const std::string& gameId) {
  qDebug() << "LobbyController: Requesting to join game with ID:" << QString::fromStdString(gameId);
  // TODO: implement
}

void LobbyServerController::wantObserveGame(const std::string& gameId) {
  qDebug() << "LobbyController: Requesting to observe game with ID:"
           << QString::fromStdString(gameId);
  // TODO: implement
}

void LobbyServerController::wantDisconnect() {
  qDebug() << "LobbyController: Requesting disconnection from lobby server.";

  socket->disconnectFromHost();
}
