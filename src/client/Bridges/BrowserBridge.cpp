#include "BrowserBridge.hpp"

void BrowserBridge::hasPageChanged(const int& pageNumber, const QList<GameInfo>& gamesList) {
  qDebug() << "BrowserBridge::hasPageChanged - Page Number:" << pageNumber
           << ", Games List Size:" << gamesList.size();
  emit onPageChanged(pageNumber, gamesList);
}

void BrowserBridge::hasErrorOccurred(const QString& message) {
  qDebug() << "BrowserBridge::hasErrorOccurred - Message:" << message;
  emit onErrorOccurred(message);
}

void BrowserBridge::callNavigateToPage(const double& page) {
  qDebug() << "BrowserBridge::callNavigateToPage - Page:" << page;
  emit requestNavigateToPage(page);
}

void BrowserBridge::callJoinGame(const QString& gameId) {
  qDebug() << "BrowserBridge::callJoinGame - Game ID:" << gameId;
  emit requestJoinGame(gameId);
}

void BrowserBridge::callObserveGame(const QString& gameId) {
  qDebug() << "BrowserBridge::callObserveGame - Game ID:" << gameId;
  emit requestObserveGame(gameId);
}
