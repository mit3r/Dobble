#include "BrowserBridge.hpp"

void BrowserBridge::callConnectToLobby(const QString& ip, const int& port) {
  qDebug() << "BrowserBridge::callConnectToLobby - IP:" << ip << ", Port:" << port;
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

void BrowserBridge::callCreateGame(const QString& gameName, const int& maxPlayers) {
  qDebug() << "BrowserBridge::callCreateGame - Game Name:" << gameName
           << ", Max Players:" << maxPlayers;
  emit requestCreateGame(gameName.toStdString(), maxPlayers);
}
