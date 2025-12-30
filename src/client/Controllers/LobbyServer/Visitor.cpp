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
  if (cmd.error.has_value()) {
    nickname.reset();
    emit hasLoginFailed(cmd.error->message);
  } else {
    emit hasLoginSucceeded(nickname.value());
  }
}

void LobbyServerController::operator()(const ResponsePingCommand&) {}

void LobbyServerController::operator()(const ResponseGetLobbyInfoCommand&) {}

void LobbyServerController::operator()(const ResponseCreateLobbyCommand&) {}

void LobbyServerController::operator()(const ResponseRegisterGameServerCommand&) {}