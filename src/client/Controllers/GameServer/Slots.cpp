#include "GameServerController.hpp"

void GameServerController::wantConnectToGame(const std::string& ip, const int& port,
                                             const std::string& gameId,
                                             const std::string& client_id,
                                             const std::string& nickname, const Role& role) {
  qDebug() << "GameServerController: Requesting connection to game server at"
           << QString::fromStdString(ip) << ":" << port;

  // Save connection info for later use
  this->gameId = gameId;
  this->clientId = client_id;
  this->nickname = nickname;
  this->role = role;

  // Connect to server
  const QString qip = ip == "0.0.0.0" ? QString("127.0.0.1") : QString::fromStdString(ip);
  socket_->connectToHost(qip, port, QIODevice::ReadWrite);
};

void GameServerController::wantMatchCard(const std::string& turnId, const int& symbolId) {
  qDebug() << "GameServerController: Requesting to match cards.";

  if (!this->clientId.has_value()) {
    qDebug() << "GameServerController: Missing client ID for matching.";
    return;
  }

  SenderMatchSymbolCommand cmd;
  cmd.command = "match_symbol";
  cmd.client_id = this->clientId.value();

  cmd.data_obj = SenderMatchSymbolCommand::data{};
  cmd.data_obj->turn_id = stoi(turnId);
  cmd.data_obj->symbol_id = symbolId;

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
};
void GameServerController::wantLeaveGame() {
  qDebug() << "GameServerController: Requesting to leave the game.";

  if (!this->clientId.has_value()) {
    qDebug() << "GameServerController: Missing client ID for leaving.";
    return;
  }

  SenderLeaveRoomCommand cmd;
  cmd.client_id = this->clientId.value();
  cmd.command = "leave_room";

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
};

void GameServerController::wantStartGame() {
  qDebug() << "GameServerController: Requesting to start the game.";
  if (!this->clientId.has_value()) {
    qDebug() << "GameServerController: Missing client ID for starting.";
    return;
  }

  SenderStartGameCommand cmd;
  cmd.client_id = this->clientId.value();
  cmd.command = "start_game";

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
};