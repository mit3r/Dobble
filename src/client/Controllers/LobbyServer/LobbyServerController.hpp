

#include <QObject>
#include <QTcpSocket>
#include <protocol/LobbyCommandFactory.hpp>
#include <protocol/lobby/lobbyroom/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>

#include "client/type/dobble.hpp"

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

signals: // Signals: controller -> ui

  // Connection signals
  void hasConnectionStateChanged(const ConnectionStatus& status);
  void hasConnectionErrorOccurred(const ConnectionError& error);

  // Communication signals
  void hasCommunicationStateChanged(const CommunicationStatus& status);

  // Login signals
  void hasLoginSucceeded(const std::string& nickname);
  void hasLoginFailed(const std::string& error);

  // Browser signals
  void hasCreatedGame(const std::string& playerName);
  void hasJoinedGame(const std::string& gameId);
  void hasObservedGame(const std::string& gameId);

  void hasReceivedPage(const std::list<ShortGameInfo>& games,
                       const std::optional<std::string>& nextPage);

public slots: // Slots: ui -> controller

  void wantConnectToServer(const std::string& ip, const int& port);

  void wantNicknameVerification(const std::string& nickname);
  void wantCreateGame(const std::string& gameName);
  void wantJoinGame(const std::string& gameId);
  void wantObserveGame(const std::string& gameId);
  void wantDisconnect();

private slots: // Slots for ui events: ui -> server

  void whenReadReady();
  void whenSocketStateChanged(QTcpSocket::SocketState socketState);
  void whenSocketError(QTcpSocket::SocketError socketError);

private:
  QTcpSocket* socket;
  LobbyCommandFactory commandFactory;
  QByteArray receiveBuffer; // Buffer for incoming data

  std::optional<std::string> nickname;
};