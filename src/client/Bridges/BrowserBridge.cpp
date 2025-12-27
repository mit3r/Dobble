#include "BrowserBridge.hpp"

void BrowserBridge::navigateToPage(const double& page) {
  qDebug() << "Navigating to page:" << page;
}

void BrowserBridge::joinGame(const QString& gameId) {
  qDebug() << "Joining game with ID:" << gameId;
}

void BrowserBridge::observeGame(const QString& gameId) {
  qDebug() << "Observing game with ID:" << gameId;
}
