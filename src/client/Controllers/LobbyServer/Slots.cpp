#include "LobbyServerController.hpp"

void LobbyServerController::wantNicknameVerification(const std::string& nickname) {
  qDebug() << "LobbyController: Requesting nickname verification for:"
           << QString::fromStdString(nickname);

  SenderLoginCommand cmd;
  cmd.command = "login";

  cmd.data_obj = SenderLoginCommand::data{};
  cmd.data_obj->nickname = nickname;

  json j = cmd;

  qDebug() << "Sending login command JSON:" << QString::fromStdString(j.dump());
  send_json_packet(socket->socketDescriptor(), json(cmd));

  connectRequestTimer([=]() { send_json_packet(socket->socketDescriptor(), json(cmd)); });
}

void LobbyServerController::wantNavigateToPage(const int& page) {
  qDebug() << "LobbyController: Requesting lobby info for page:" << page;

  SenderGetLobbyInfoCommand cmd;
  cmd.command = "getinfolobby";
  cmd.data_obj = SenderGetLobbyInfoCommand::data{};
  cmd.data_obj->page = std::to_string(page);

  send_json_packet(socket->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket->socketDescriptor(), json(cmd)); });
}

void LobbyServerController::wantCreateGame(const std::string& gameName, const int& maxPlayers) {
  qDebug() << "LobbyController: Requesting game creation with name:"
           << QString::fromStdString(gameName) << ", max players:" << maxPlayers;

  SenderCreateLobbyCommand cmd;
  cmd.command = "create_lobby";
  cmd.data_obj = SenderCreateLobbyCommand::data{};
  cmd.data_obj->game_name = gameName;

  send_json_packet(socket->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket->socketDescriptor(), json(cmd)); });
}

void LobbyServerController::wantDisconnect() {
  qDebug() << "LobbyController: Requesting disconnection from lobby server.";

  socket->disconnectFromHost();
}
