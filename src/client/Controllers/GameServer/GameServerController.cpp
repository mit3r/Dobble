#include "GameServerController.hpp"

void GameServerController::joinGame() {
  qDebug() << "GameServerController: Requesting to join game"
           << QString::fromStdString(gameId.value());

  if (!this->gameId.has_value() || !this->clientId.has_value() || !this->nickname.has_value() ||
      !this->role.has_value()) {
    qDebug() << "GameServerController: Missing information to join game.";
    emit hasConnectGameFailed("Missing information to join game.");
    return;
  }

  SenderJoinGameCommand cmd;
  cmd.command = "join_game";
  cmd.client_id = this->clientId.value();
  cmd.client_nickname = this->nickname.value();

  cmd.data_obj = SenderJoinGameCommand::data{};
  cmd.data_obj->game_id = this->gameId.value();
  cmd.data_obj->role = (this->role.value() == Role::Player) ? "player" : "observer";

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
}

void GameServerController::leaveGame() {
  qDebug() << "GameServerController: Disconnecting from game server.";

  this->clientId.reset();
  this->gameId.reset();
  this->nickname.reset();
  this->role.reset();

  socket_->disconnectFromHost();
}

void GameServerController::handleRequestTimeout() {
  switch (requestCounter++) {
  case 1:
    emit hasCommunicationStateChanged(CommunicationStatus::Retrying);
    break;
  case 5:
    emit hasCommunicationStateChanged(CommunicationStatus::Failed);
    disconnectRequestTimer(true);
    leaveGame();
    break;
  }
}

void GameServerController::handleServerPing() {
  if (!this->clientId.has_value()) {
    qDebug() << "GameServerController: Missing client ID for ping.";
    return;
  }

  SenderGameClientPingCommand pingCmd;
  pingCmd.command = "ping";
  pingCmd.client_id = this->clientId.value();
  pingCmd.data_obj = SenderGameClientPingCommand::data{};
  pingCmd.data_obj->message = "ping";

  send_json_packet(socket_->socketDescriptor(), json(pingCmd));
}

void GameServerController::handleServerInfo() {
  SenderSendGameInfoCommand infoCmd;
  infoCmd.command = "send_game_info";
  send_json_packet(socket_->socketDescriptor(), json(infoCmd));
}