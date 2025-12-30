

#include <QObject>
#include <QTcpSocket>
#include <protocol/LobbyCommandFactory.hpp>
#include <protocol/lobby/lobbyroom/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>

#pragma once

/**
 * @brief Manages the communication with lobby server to start/join games.
 */
class LobbyServerController : public QObject {
  Q_OBJECT
  public:
  explicit LobbyServerController(QObject* parent = nullptr);

  // Visitor operators for handling commands from variant
  template <typename T>
  void operator()(const T& cmd);

  void operator()(const std::monostate&);
  void operator()(const ResponseLoginCommand& cmd);
  void operator()(const ResponsePingCommand& cmd);
  void operator()(const ResponseJoinGameCommand& cmd);
  void operator()(const ResponseGetLobbyInfoCommand& cmd);
  void operator()(const ResponseCreateLobbyCommand& cmd);
  void operator()(const ResponseRegisterGameServerCommand& cmd);
  void operator()(const ResponseLeaveRoomCommand& cmd);
  void operator()(const ResponseSendGameInfoCommand& cmd);
  void operator()(const SenderLoginCommand& cmd);
  void operator()(const SenderPingCommand& cmd);
  void operator()(const SenderJoinGameCommand& cmd);
  void operator()(const SenderGetLobbyInfoCommand& cmd);
  void operator()(const SenderCreateLobbyCommand& cmd);
  void operator()(const SenderLeaveRoomCommand& cmd);
  void operator()(const SenderSendGameInfoCommand& cmd);

  signals:  // Signals: controller -> ui

      void hasLoginSucceeded(const QString& nickname);
      void hasLoginFailed(const QString& error);

      void hasCreatedGame(const QString& playerName);

      void hasJoinedGame(const QString& gameId);

      void hasObservedGame(const QString& gameId);

      void hasDisconnected();

      void hasErrorOccurred(const QString& errorMessage);

  private slots:  // Slots for ui events: ui -> server

  void whenReadReady();

  public slots:  // Slots: ui -> controller

      void wantNicknameVerification(const QString& nickname);

      void wantCreateGame(const QString& gameName);

      void wantJoinGame(const QString& gameId);

      void wantObserveGame(const QString& gameId);

      void wantDisconnect();

  private:
  QTcpSocket* socket;
  LobbyCommandFactory commandFactory;
  std::optional<QString> currentNickname;
  QByteArray receiveBuffer;  // Buffer for incoming data
};