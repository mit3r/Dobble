
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
  void hasGameInfoUpdated(const QVariantMap& gameInfo);
  void hasMatchedCardResult(bool isCorrect);
  void hasLeftGame();

public slots: // Slots called when game server responds
  void wantConnect(const std::string& ip, const int& port);
  void wantMatchCard(int topCard, int topPick, int handPick);
  void wantLeaveGame();

private slots:
  void whenReadReady();
  void whenSocketStateChanged(QTcpSocket::SocketState socketState);
  void whenSocketError(QTcpSocket::SocketError socketError);

private:
  QTcpSocket* socket_;
  GameCommandFactory commandFactory;
  QByteArray receiveBuffer;
};