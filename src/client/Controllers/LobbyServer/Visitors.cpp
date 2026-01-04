#include "LobbyServerController.hpp"

template <typename T> inline void LobbyServerController::operator()(const T& cmd) {
  qDebug() << "LobbyServerController: got unknown command type";
}

void LobbyServerController::operator()(const std::monostate&) {
  qDebug() << "LobbyServerController: got monostate";
}

void LobbyServerController::operator()(const ResponseLoginCommand& cmd) {
  qDebug() << "LobbyServerController: got ResponseLoginCommand";

  if (cmd.error.has_value()) {

    if (cmd.error->code == 400) {
      emit hasAlreadyLoggedIn();
    } else {
      emit hasLoginFailed(cmd.error->message);
    }
    return;
  }

  if (cmd.data_obj.has_value()) {
    emit hasLoginSucceeded(cmd.client_id.value());
  }
}

void LobbyServerController::operator()(const ResponseGetLobbyInfoCommand& cmd) {
  qDebug() << "LobbyServerController: got ResponseGetLobbyInfoCommand";

  QVariantList gamesList;
  if (cmd.error.has_value()) {
    // TODO: handle error properly
    // with communication retry, etc.

    qDebug() << "Error in ResponseGetLobbyInfoCommand:"
             << QString::fromStdString(cmd.error->message);

    emit hasReceivedPage(gamesList, 1, 0);
    return;
  }

  int currPage = stoi(cmd.data_obj->page);
  int nextPage = cmd.data_obj->next_page.has_value() ? stoi(cmd.data_obj->next_page.value()) : 0;

  for (const auto& game : cmd.data_obj->actual_games) {
    CShortGameInfo shortInfo;
    shortInfo.gameId = QString::fromStdString(game.game_id);
    shortInfo.gameName = QString::fromStdString(game.game_name);
    shortInfo.players = stoi(game.players);
    shortInfo.maxPlayers = stoi(game.max_players);
    shortInfo.ip = QString::fromStdString(game.ip);
    shortInfo.port = game.port;

    if (game.status == "INIT" || game.status == "WAITING")
      shortInfo.status = GameStatus::Waiting;
    else if (game.status == "GAME_ACTIVE")
      shortInfo.status = GameStatus::InGame;
    else // "GAME_OVER"
      shortInfo.status = GameStatus::Finished;

    gamesList.append(shortInfo.toVariantMap());
  }

  emit hasReceivedPage(gamesList, currPage, nextPage);
}

void LobbyServerController::operator()(const ResponseCreateLobbyCommand&) {}

void LobbyServerController::operator()(const ResponseRegisterGameServerCommand&) {}