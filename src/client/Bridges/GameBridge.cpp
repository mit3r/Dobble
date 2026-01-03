#include "GameBridge.hpp"

void GameBridge::callJoinGame(const QString& ip, const int& port, const QString& gameId,
                              const Role& role) {
  qDebug() << "GameBridge::callJoinGame called with" << ip << port << gameId;
  emit requestJoinToGame(ip.toStdString(), port, gameId.toStdString(), role);
}

void GameBridge::callMatch(const QString& turnId, const int& symbolId) {
  qDebug() << "GameBridge::callMatch called with" << turnId << symbolId;
  emit requestMatch(turnId.toStdString(), symbolId);
}

void GameBridge::callQuitGame() {
  qDebug() << "GameBridge::callQuitGame called";
  emit requestQuitGame();
}
