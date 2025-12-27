#include "LobbyServerController.hpp"

void LobbyServerController::operator()(const std::monostate&) {
}

// Response commands (from server to client)
void LobbyServerController::operator()(const ResponseLoginCommand& cmd) {
  if (cmd.error.has_value()) return emit hasErrorOccurred(QString::fromStdString(cmd.error->message));

  emit hasVerifiedNickname();
}

void LobbyServerController::operator()(const ResponsePingCommand& cmd) {
}

void LobbyServerController::operator()(const ResponseJoinGameCommand& cmd) {
}

void LobbyServerController::operator()(const ResponseGetLobbyInfoCommand& cmd) {
}

void LobbyServerController::operator()(const ResponseCreateLobbyCommand& cmd) {
}

void LobbyServerController::operator()(const ResponseRegisterGameServerCommand& cmd) {
}

void LobbyServerController::operator()(const ResponseLeaveRoomCommand& cmd) {
}

void LobbyServerController::operator()(const ResponseSendGameInfoCommand& cmd) {
}

// Sender commands (these shouldn't normally be received, but variant requires them)
void LobbyServerController::operator()(const SenderLoginCommand& cmd) {
  Q_UNUSED(cmd);
}

void LobbyServerController::operator()(const SenderPingCommand& cmd) {
  Q_UNUSED(cmd);
}

void LobbyServerController::operator()(const SenderJoinGameCommand& cmd) {
  Q_UNUSED(cmd);
}

void LobbyServerController::operator()(const SenderGetLobbyInfoCommand& cmd) {
  Q_UNUSED(cmd);
}

void LobbyServerController::operator()(const SenderCreateLobbyCommand& cmd) {
  Q_UNUSED(cmd);
}

void LobbyServerController::operator()(const SenderLeaveRoomCommand& cmd) {
  Q_UNUSED(cmd);
}

void LobbyServerController::operator()(const SenderSendGameInfoCommand& cmd) {
  Q_UNUSED(cmd);
}