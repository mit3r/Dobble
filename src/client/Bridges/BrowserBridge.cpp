#include "BrowserBridge.hpp"

void BrowserBridge::callVerifyNickname(const std::string& nickname) {
  qDebug() << "BrowserBridge::callVerifyNickname - Nickname:" << nickname.c_str();
  emit requestVerifyNickname(nickname);
}

void BrowserBridge::callNavigateToPage(const double& page) {
  qDebug() << "BrowserBridge::callNavigateToPage - Page:" << page;
  emit requestNavigateToPage(page);
}

void BrowserBridge::callJoinGame(const std::string& gameId) {
  qDebug() << "BrowserBridge::callJoinGame - Game ID:" << gameId.c_str();
  emit requestJoinGame(gameId);
}

void BrowserBridge::callObserveGame(const std::string& gameId) {
  qDebug() << "BrowserBridge::callObserveGame - Game ID:" << gameId.c_str();
  emit requestObserveGame(gameId);
}
