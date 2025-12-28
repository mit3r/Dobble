#include "GameBridge.hpp"

void GameBridge::hasPlayerStatusChanged(const QString& nickname, const QString& status) {
  qDebug() << "GameBridge::hasPlayerStatusChanged called with"
           << nickname << status;
  emit onPlayerStatusChanged(nickname, status);
}

void GameBridge::hasPlayerScoreChanged(const QString& nickname, const int& score) {
  qDebug() << "GameBridge::hasPlayerScoreChanged called with"
           << nickname << score;
  emit onPlayerScoreChanged(nickname, score);
}

void GameBridge::hasTopCardChanged(const int& cardId) {
  qDebug() << "GameBridge::hasTopCardChanged called with"
           << cardId;
  emit onTopCardChanged(cardId);
}

void GameBridge::hasPlayerCardChanged(const QString& nickname, const int& cardId) {
  qDebug() << "GameBridge::hasPlayerCardChanged called with"
           << nickname << cardId;
  emit onPlayerCardChanged(nickname, cardId);
}

void GameBridge::hasLastsCardsChanged(const int& count) {
  qDebug() << "GameBridge::hasLastsCardsChanged called with"
           << count;
  emit onLastsCardsChanged(count);
}

void GameBridge::hasQuitGame() {
  qDebug() << "GameBridge::hasQuitGame called";
  emit onGameQuit();
}

void GameBridge::callMatch(const int& pick1, const int& pick2) {
  qDebug() << "GameBridge::callMatch called with"
           << pick1 << pick2;
  emit requestMatch(pick1, pick2);
}

void GameBridge::callQuitGame() {
  qDebug() << "GameBridge::callQuitGame called";
  emit requestQuitGame();
}
