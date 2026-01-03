#include "GameServerController.hpp"

template <typename T> inline void GameServerController::operator()(const T&) {
  qDebug() << "GameServerController: got unknown command type";
}

void GameServerController::operator()(const std::monostate&) {
  qDebug() << "GameServerController: got monostate";
}

void GameServerController::operator()(const ResponseGameClientPingCommand&) {
  qDebug() << "GameServerController: got ResponseGameClientPingCommand";
}

void GameServerController::operator()(const ResponseJoinGameCommand& cmd) {
  qDebug() << "GameServerController: got ResponseJoinGameCommand";

  qDebug() << "Join game response received:" << json(cmd).dump().c_str();

  if (!cmd.error.has_value()) {
    emit hasConnectGameSucceed();
  } else {
    emit hasConnectGameFailed(QString::fromStdString(cmd.error->message));
  }
}

void GameServerController::operator()(const ResponseLeaveRoomCommand& cmd) {
  qDebug() << "GameServerController: got ResponseLeaveRoomCommand";

  if (cmd.data_obj.has_value()) {
    const auto& data = cmd.data_obj.value();
    qDebug() << "Left game with message:" << QString::fromStdString(data.message);
    emit hasLeftGame();
  } else {
    qDebug() << "Left game with no message.";
    emit hasLeftGame();
  }
}
void GameServerController::operator()(const ResponseSendGameInfoCommand& cmd) {
  qDebug() << "GameServerController: got ResponseSendGameInfoCommand";

  if (!cmd.data_obj.has_value()) {
    qDebug() << "No game info data received.";
    return;
  }

  auto& game = cmd.data_obj.value();
  CGameInfo gameInfo;

  gameInfo.gameId = QString::fromStdString(game.game_id);
  gameInfo.winnerNickname = QString::fromStdString(game.actual_turn.winner_id);
  gameInfo.topCardId = game.actual_turn.topCardId;
  gameInfo.lastsCards = game.actual_turn.cardsLeft;
  gameInfo.turnId = QString::fromStdString(game.actual_turn.turn_id);

  if (game.actual_turn.status == "GAME_ACTIVE") {
    gameInfo.status = GameStatus::InGame;
  } else if (game.actual_turn.status == "GAME_FINISHED") {
    gameInfo.status = GameStatus::Finished;
  } else {
    gameInfo.status = GameStatus::Waiting;
  }

  QVariantList playersList;
  for (const auto& player : game.actual_turn.clients_data) {
    CPlayerGameInfo playerInfo;
    playerInfo.nickname = QString::fromStdString(player.nickname);
    playerInfo.cardId = player.cardId;
    playerInfo.points = player.score;
    playerInfo.matches = player.points;
    playerInfo.mistakes = player.mistakes;
    gameInfo.players.append(playerInfo);
  }

  emit hasGameInfoUpdated(gameInfo.toVariantMap());
}
void GameServerController::operator()(const ResponseMatchSymbolCommand& cmd) {
  qDebug() << "GameServerController: got ResponseMatchSymbolCommand";

  if (!cmd.data_obj.has_value()) {
    qDebug() << "No match symbol data received.";
    return;
  }

  auto& data = cmd.data_obj.value();
  if (data.success) {
    qDebug() << "Match correct! Points awarded:" << data.points_awarded
             << "New score:" << data.new_score;
    emit hasMatchResult(true);
  } else {
    qDebug() << "Match incorrect.";
    emit hasMatchResult(false);
  }
}
void GameServerController::operator()(const ResponseStartGameCommand& cmd) {
  qDebug() << "GameServerController: got ResponseStartGameCommand";

  if (!cmd.error.has_value()) {
    emit hasGameStartedSucceed();
  } else {
    emit hasGameStartedFailed(QString::fromStdString(cmd.error->message));
  }
}
