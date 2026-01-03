#include "GameServerController.hpp"

void GameServerController::wantConnectToGame(const std::string& ip, const int& port,
                                             const std::string& gameId,
                                             const std::string& client_id,
                                             const std::string& nickname, const Role& role) {
  qDebug() << "GameServerController: Requesting connection to game server at"
           << QString::fromStdString(ip) << ":" << port;

  connect(socket_, &QTcpSocket::connected, this,
          [=]() { joinGame(gameId, client_id, nickname, role); });

  // Connect to server
  socket_->connectToHost(QString::fromStdString(ip), static_cast<quint16>(port));
};

void GameServerController::wantMatchCard(const std::string& gameId, const std::string& turnId,
                                         const int& symbolId) {
  qDebug() << "GameServerController: Requesting to match cards.";

  SenderMatchSymbolCommand cmd;
  cmd.command = "match_symbol";

  cmd.data_obj = SenderMatchSymbolCommand::data{};
  cmd.data_obj->game_id = gameId;
  cmd.data_obj->turn_id = stoi(turnId);
  cmd.data_obj->symbol_id = symbolId;

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
};
void GameServerController::wantLeaveGame() {
  qDebug() << "GameServerController: Requesting to leave the game.";
  SenderLeaveRoomCommand cmd;
  cmd.command = "leave_game";

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
};

void GameServerController::wantStartGame() {
  qDebug() << "GameServerController: Requesting to start the game.";
  SenderStartGameCommand cmd;
  cmd.command = "start_game";
  cmd.data_obj = SenderStartGameCommand::data{};

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
};