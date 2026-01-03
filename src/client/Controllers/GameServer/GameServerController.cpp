#include "GameServerController.hpp"

void GameServerController::joinGame(const std::string& gameId, const std::string& client_id,
                                    const std::string& nickname, const Role& role) {
  qDebug() << "GameServerController: Requesting to join game with ID:"
           << QString::fromStdString(gameId);

  SenderJoinGameCommand cmd;
  cmd.command = "join_game";
  cmd.client_id = client_id;
  cmd.client_nickname = nickname;

  cmd.data_obj = SenderJoinGameCommand::data{};
  cmd.data_obj->game_id = gameId;
  cmd.data_obj->role = (role == Role::Player) ? "player" : "observer";

  send_json_packet(socket_->socketDescriptor(), json(cmd));
  connectRequestTimer([=]() { send_json_packet(socket_->socketDescriptor(), json(cmd)); });
}