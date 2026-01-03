
#include <QObject>
#include <QTcpSocket>
#include <QTimer>
#include <protocol/GameCommandFactory.hpp>
#include <protocol/lobby/room/SocketCommands.hpp>
#include <protocol/utils/SendAndReceiveUtils.hpp>
#include "Controllers/ControllersUtils.hpp"

#include "client/type/dobble.hpp"

#pragma once

/**
 * @brief Controller responsible for communication with the game server.
 */
class GameServerController : public QObject {
  Q_OBJECT
public:
  explicit GameServerController(QObject* parent = nullptr);

  // Visitor operators for handling commands from variant
  template <typename T = GameClientCommand> void operator()(const T& cmd);
  void operator()(const std::monostate&);
  inline void operator()(const RoomServerReceivedMessage&) {}
  inline void operator()(const RoomClientReceivedMessage&) {}

  void operator()(const ResponseGameClientPingCommand& cmd);
  void operator()(const ResponseJoinGameCommand& cmd);
  void operator()(const ResponseLeaveRoomCommand& cmd);
  void operator()(const ResponseSendGameInfoCommand& cmd);
  void operator()(const ResponseMatchSymbolCommand& cmd);
  void operator()(const ResponseStartGameCommand& cmd);

  inline void operator()(const SenderGameClientPingCommand&) {};
  inline void operator()(const SenderJoinGameCommand&) {};
  inline void operator()(const SenderLeaveRoomCommand&) {};
  inline void operator()(const SenderSendGameInfoCommand&) {};
  inline void operator()(const SenderMatchSymbolCommand&) {};
  inline void operator()(const SenderStartGameCommand&) {};

signals: // Signals: controller -> ui

  // Connection signals
  void hasConnectionStateChanged(const ConnectionStatus& status);
  void hasConnectionErrorOccured(const ConnectionError& error);

  // Communication signals
  void hasCommunicationStateChanged(const CommunicationStatus& status);

  // Game signals
  void hasConnectGameSucceed();
  void hasConnectGameFailed(const QString& error);

  void hasGameStartedSucceed();
  void hasGameStartedFailed(const QString& error);

  void hasMatchResult(const bool& correct);

  void hasGameInfoUpdated(const QVariantMap& gameInfo);
  void hasLeftGame();

public slots: // Slots: ui -> controller
  void wantConnectToGame(const std::string& ip, const int& port, const std::string& gameId,
                         const std::string& client_id, const std::string& nickname,
                         const Role& role);
  void wantStartGame();

  void wantMatchCard(const std::string& gameId, const std::string& turnId, const int& symbolId);
  void wantLeaveGame();

private slots:
  void whenReadReady();
  void whenSocketStateChanged(QTcpSocket::SocketState socketState);
  void whenSocketError(QTcpSocket::SocketError socketError);

private:
  QTcpSocket* socket_;
  GameCommandFactory commandFactory;
  QByteArray receiveBuffer;

  int requestCounter = 0;
  QTimer* requestTimer = new QTimer(this);
  void connectRequestTimer(std::function<void()> slot);
  void disconnectRequestTimer(bool failed);

  QTimer* pingTimer = new QTimer(this);

  void joinGame(const std::string& gameId, const std::string& client_id,
                const std::string& nickname, const Role& role);
};