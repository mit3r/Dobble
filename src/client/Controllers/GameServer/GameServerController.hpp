
#include <QObject>
#include <QTcpSocket>

#pragma once

/**
 * @brief Controller responsible for communication with the game server.
 */
class GameServerController : public QObject {
  Q_OBJECT
  public:
  explicit GameServerController(QObject* parent = nullptr);

  signals:  // Signals emmitted to game server

  // Request to start the game
  void setReady(bool isReady);

  // Request to play a card
  void matchCard(int pick1, int pick2);

  // Request to leave the game
  void leaveGame();

  public slots:  // Slots called when game server responds

  // Handle update about player's readiness
  void onPlayerReadyUpdated(int playerId, bool isReady);

  // Handle game started notification
  void onGameStarted();

  // Handle card match result
  void onCardMatched(int playerId, bool isCorrect);

  // Handle score update
  void onScoreUpdated(int playerId, int newScore);

  // Handle new top card notification
  void onNewTopCard(const QString& cardData);

  // Handle player's hand update
  void onHandUpdated(int playerId, int cardId);

  // Handle lasts cards counter
  void onLastCardsUpdated(int count);

  // Handle game ended notification
  void onGameEnded(
      const QList<QString>& playerNames,
      const QList<int>& playerScores);

  // Handle disconnection from the server
  void onDisconnected();

  // Handle errors
  void onError(QAbstractSocket::SocketError socketError);

  private:
  QTcpSocket* socket_;
};