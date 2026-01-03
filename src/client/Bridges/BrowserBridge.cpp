#include "BrowserBridge.hpp"

void BrowserBridge::callConnectToLobbyServer(const QString& ip, const int& port) {
  qDebug() << "BrowserBridge::callConnectToLobbyServer - IP:" << ip << ", Port:" << port;
  emit requestConnectToLobbyServer(ip.toStdString(), port);
}

void BrowserBridge::callVerifyNickname(const QString& nickname) {
  qDebug() << "BrowserBridge::callVerifyNickname - Nickname:" << nickname;
  emit requestVerifyNickname(nickname.toStdString());
}

void BrowserBridge::callNavigateToPage(const int& page) {
  qDebug() << "BrowserBridge::callNavigateToPage - Page:" << page;
  emit requestNavigateToPage(page);
}

void BrowserBridge::callJoinGame(const QString& gameId) {
  qDebug() << "BrowserBridge::callJoinGame - Game ID:" << gameId;
  emit requestJoinGame(gameId.toStdString());
}

void BrowserBridge::callObserveGame(const QString& gameId) {
  qDebug() << "BrowserBridge::callObserveGame - Game ID:" << gameId;
  emit requestObserveGame(gameId.toStdString());
}

void BrowserBridge::callCreateGame(const QString& gameName, const int& maxPlayers) {
  qDebug() << "BrowserBridge::callCreateGame - Game Name:" << gameName
           << ", Max Players:" << maxPlayers;
  emit requestCreateGame(gameName.toStdString(), maxPlayers);
}
