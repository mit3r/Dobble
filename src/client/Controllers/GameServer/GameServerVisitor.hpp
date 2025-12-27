

#pragma once

class GameServerVisitor {
  public:
  virtual ~GameServerVisitor() = default;

  virtual void visitPlayerReadyUpdated(int playerId, bool isReady) = 0;
  virtual void visitGameStarted() = 0;
  virtual void visitCardMatched(int playerId, bool isCorrect) = 0;
  virtual void visitScoreUpdated(int playerId, int newScore) = 0;
  virtual void visitNewTopCard(const QString& cardData) = 0;
  virtual void visitHandUpdated(int playerId, int cardId) = 0;
  virtual void visitLastCardsUpdated(int count) = 0;
  virtual void visitGameEnded(
      const QList<QString>& playerNames,
      const QList<int>& playerScores) = 0;
};