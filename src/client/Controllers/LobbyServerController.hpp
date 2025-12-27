

#include <QObject>
#include <QTcpSocket>

#pragma once

/**
 * @brief Manages the communication with lobby server to start/join games.
 */
class LobbyServerController : public QObject {
  Q_OBJECT
  public:
  explicit LobbyServerController(QObject* parent = nullptr);

  signals:  // Signals emmitted to lobby server

  // Request a nickname's verification
  void verifyNickname(const QString& nickname);

  // Request to create a new game
  void createGame(const QString& playerName);

  // Request to join an existing game
  void joinGame(const QString& gameId);

  // Request to observe an existing game
  void observeGame(const QString& gameId);

  // Request to close the connection
  void close();

  public slots:  // Slots called when lobby server responds

  // Handle nickname verification response
  void onNicknameVerified(bool isAvailable);

  // Handle game creation response
  void onGameCreated(const QString& gameId);

  // Handle game joining response
  void onGameJoined(bool success, const QString& reason);

  // Handle game observing response
  void onGameObserved(bool success, const QString& reason);

  // Handle disconnection from the server
  void onDisconnected();

  // Handle errors
  void onError(QAbstractSocket::SocketError socketError);

  private:
  QTcpSocket* socket_;
};