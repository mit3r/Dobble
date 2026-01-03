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

void GameServerController::operator()(const ResponseJoinGameCommand&) {
  qDebug() << "GameServerController: got ResponseJoinGameCommand";
}

void GameServerController::operator()(const ResponseLeaveRoomCommand&) {
  qDebug() << "GameServerController: got ResponseLeaveRoomCommand";
}
void GameServerController::operator()(const ResponseSendGameInfoCommand&) {
  qDebug() << "GameServerController: got ResponseSendGameInfoCommand";
}
void GameServerController::operator()(const ResponseMatchSymbolCommand&) {
  qDebug() << "GameServerController: got ResponseMatchSymbolCommand";
}
void GameServerController::operator()(const ResponseStartGameCommand&) {
  qDebug() << "GameServerController: got ResponseStartGameCommand";
}