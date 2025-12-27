#include "GameServerController.hpp"

GameServerController::GameServerController(QObject* parent) {
  Q_UNUSED(parent);

  socket_ = new QTcpSocket(this);
}

void GameServerController::onPlayerReadyUpdated(int playerId, bool isReady) {
  // TODO: Implement player ready update logic
  Q_UNUSED(playerId);
  Q_UNUSED(isReady);
}

void GameServerController::onGameStarted() {
  // TODO: Implement game started logic
}

void GameServerController::onCardMatched(int playerId, bool isCorrect) {
  // TODO: Implement card matched logic
  Q_UNUSED(playerId);
  Q_UNUSED(isCorrect);
}

void GameServerController::onScoreUpdated(int playerId, int newScore) {
  // TODO: Implement score updated logic
  Q_UNUSED(playerId);
  Q_UNUSED(newScore);
}

void GameServerController::onNewTopCard(const QString& cardData) {
  // TODO: Implement new top card logic
  Q_UNUSED(cardData);
}

void GameServerController::onHandUpdated(int playerId, int cardId) {
  // TODO: Implement hand updated logic
  Q_UNUSED(playerId);
  Q_UNUSED(cardId);
}

void GameServerController::onLastCardsUpdated(int count) {
  // TODO: Implement last cards updated logic
  Q_UNUSED(count);
}

void GameServerController::onGameEnded(
    const QList<QString>& playerNames,
    const QList<int>& playerScores) {
  // TODO: Implement game ended logic
  Q_UNUSED(playerNames);
  Q_UNUSED(playerScores);
}

void GameServerController::onDisconnected() {
  // TODO: Implement disconnection logic
}

void GameServerController::onError(QAbstractSocket::SocketError socketError) {
  // TODO: Implement error handling logic
  Q_UNUSED(socketError);
}