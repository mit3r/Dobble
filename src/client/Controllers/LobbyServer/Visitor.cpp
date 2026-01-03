#include "LobbyServerController.hpp"

template <typename T>
inline void LobbyServerController::operator()(const T&) {
  qDebug() << "LobbyServerController: got unknown command type";
}

void LobbyServerController::operator()(const std::monostate&) {
  qDebug() << "LobbyServerController: got monostate";
}

void LobbyServerController::operator()(const ResponseLoginCommand& cmd) {
  qDebug() << "LobbyServerController: got ResponseLoginCommand";
  qDebug() << "ResponseLoginCommand content:" << QString::fromStdString(json(cmd).dump());

  if (cmd.error.has_value()) {

    if (cmd.error->code == 400)
      emit hasAlreadyLoggedIn();
    else {
      emit hasLoginFailed(cmd.error->message);
    }
  } else {
    emit hasLoginSucceeded();
  }
}

void LobbyServerController::operator()(const ResponsePingCommand&) {}

void LobbyServerController::operator()(const ResponseGetLobbyInfoCommand& cmd) {
  qDebug() << "LobbyServerController: got ResponseGetLobbyInfoCommand";
  qDebug() << "ResponseGetLobbyInfoCommand content:" << QString::fromStdString(json(cmd).dump());

  if (cmd.error.has_value()) {
    // TODO: handle error properly
    // with communication retry, etc.

    qDebug() << "Error in ResponseGetLobbyInfoCommand:"
             << QString::fromStdString(cmd.error->message);
    return;
  }

  int nextPage = cmd.data_obj->next_page.has_value() ? stoi(cmd.data_obj->next_page.value()) : 0;

  QVariantList gamesList;
  for (const auto& game : cmd.data_obj->actual_games) {
    CShortGameInfo shortInfo;
    shortInfo.gameId = QString::fromStdString(game.game_id);
    shortInfo.gameName = QString::fromStdString(game.game_name);
    shortInfo.players = stoi(game.players);
    shortInfo.maxPlayers = stoi(game.max_players);

    if (game.status == "waiting")
      shortInfo.status = GameStatus::Waiting;
    else if (game.status == "in_game")
      shortInfo.status = GameStatus::InGame;
    else
      shortInfo.status = GameStatus::Finished;

    gamesList.append(shortInfo.toVariantMap());
  }

  emit hasReceivedPage(gamesList, nextPage);
}

void LobbyServerController::operator()(const ResponseCreateLobbyCommand&) {}

void LobbyServerController::operator()(const ResponseRegisterGameServerCommand&) {}