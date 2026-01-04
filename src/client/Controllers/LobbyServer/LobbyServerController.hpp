

#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <protocol/LobbyCommandFactory.hpp>
#include <protocol/lobby/lobbyroom/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include "Controllers/ControllersUtils.hpp"

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
  template <typename T = LobbyClientCommand> void operator()(const T& cmd);

  void operator()(const std::monostate&);
  void operator()(const ResponseLoginCommand& cmd);
  void operator()(const ResponseGetLobbyInfoCommand& cmd);
  void operator()(const ResponseCreateLobbyCommand& cmd);
  void operator()(const ResponseRegisterGameServerCommand& cmd);

  inline void operator()(const ResponsePingCommand&) {};
  inline void operator()(const SenderLoginCommand&) {};
  inline void operator()(const SenderPingCommand&) {};
  inline void operator()(const SenderGetLobbyInfoCommand&) {};
  inline void operator()(const SenderCreateLobbyCommand&) {};
  inline void operator()(const SenderRegisterGameServerCommand&) {};

signals: // Signals: controller -> ui

  // Connection signals
  void hasConnectionStateChanged(const ConnectionStatus& status);
  void hasConnectionErrorOccured(const ConnectionError& error);

  // Communication signals
  void hasCommunicationStateChanged(const CommunicationStatus& status);

  // Login signals
  void hasLoginSucceeded(const std::string& clientId);
  void hasLoginFailed(const std::string& error);
  void hasAlreadyLoggedIn();

  // Browser signals
  void hasCreatedGame(const std::string& playerName);
  void hasReceivedPage(const QVariantList& games, const int& currPage, const int& nextPage);

public slots: // Slots: ui -> controller

  void wantConnectToServer(const std::string& ip, const int& port);

  void wantNicknameVerification(const std::string& nickname);
  void wantNavigateToPage(const int& page);
  void wantCreateGame(const std::string& gameName, const int& maxPlayers);
  void wantDisconnect();

private slots: // Slots for ui events: ui -> server

  void whenReadReady();
  void whenSocketStateChanged(QTcpSocket::SocketState socketState);
  void whenSocketError(QTcpSocket::SocketError socketError);

private:
  QTcpSocket* socket;
  LobbyCommandFactory commandFactory;
  QByteArray receiveBuffer; // Buffer for incoming data

  int requestCounter = 0;
  QTimer* requestTimer = new QTimer(this);
  void connectRequestTimer(std::function<void()> slot);
  void disconnectRequestTimer(bool failed);
};
