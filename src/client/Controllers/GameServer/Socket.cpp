#include "GameServerController.hpp"
#include <iostream>

GameServerController::GameServerController(QObject* parent) {
  Q_UNUSED(parent);
  socket_ = new QTcpSocket(this);
  commandFactory = GameCommandFactory();

  connect(socket_, &QTcpSocket::readyRead, this, &GameServerController::whenReadReady);
  connect(socket_, &QTcpSocket::stateChanged, this, &GameServerController::whenSocketStateChanged);
  connect(socket_, &QTcpSocket::errorOccurred, this, &GameServerController::whenSocketError);
  connect(socket_, &QTcpSocket::connected, this, &GameServerController::joinGame);

  connect(requestTimer, &QTimer::timeout, this, &GameServerController::handleRequestTimeout);

  connect(pingTimer, &QTimer::timeout, this, &GameServerController::handleServerPing);
  connect(infoTimer, &QTimer::timeout, this, &GameServerController::handleServerInfo);

  connect(this, &GameServerController::hasConnectionStateChanged,
          [=](const ConnectionStatus& status) {
            if (status == ConnectionStatus::Connected) {
              infoTimer->start(500);
              handleServerInfo();

              pingTimer->start(30000);
              handleServerPing();
              return;
            }
            pingTimer->stop();
            infoTimer->stop();
          });
}

void GameServerController::whenReadReady() {
  receiveBuffer.append(socket_->readAll());

  auto packetOpt = try_extract_packet(receiveBuffer);
  if (!packetOpt.has_value())
    return;

  json packet = packetOpt.value();

  if (!packet.contains("command")) {
    std::cout << "[CLIENT ERROR] Received packet without command field." << std::endl;
    return;
  }

  qDebug() << "GameServerController: Received packet: " << QString::fromStdString(packet.dump());

  disconnectRequestTimer(false); // Successfully received a packet
  std::visit(*this, this->commandFactory.get(packet["command"], packet));
}

void GameServerController::whenSocketStateChanged(QTcpSocket::SocketState socketState) {
  qDebug() << "GameServerController: Socket state changed to" << static_cast<int>(socketState);

  std::optional<ConnectionStatus> state;
  switch (socketState) {
  case QTcpSocket::ConnectedState:
    state = ConnectionStatus::Connected;
    break;
  case QTcpSocket::ConnectingState:
    state = ConnectionStatus::Connecting;
    break;
  case QTcpSocket::UnconnectedState:
    state = ConnectionStatus::Disconnected;
    break;
  case QTcpSocket::HostLookupState:
    state = ConnectionStatus::HostLookup;
    break;
  default:
    break;
  };

  if (state.has_value())
    emit this->hasConnectionStateChanged(state.value());
}

void GameServerController::whenSocketError(QTcpSocket::SocketError socketError) {
  qDebug() << "GameServerController: Socket error occurred:" << static_cast<int>(socketError);

  std::optional<ConnectionError> error;
  switch (socketError) {
  case QTcpSocket::ConnectionRefusedError:
    error = ConnectionError::ConnectionRefused;
    break;
  case QTcpSocket::HostNotFoundError:
    error = ConnectionError::HostNotFound;
    break;
  case QTcpSocket::NetworkError:
    error = ConnectionError::NetworkError;
    break;
  case QTcpSocket::SocketTimeoutError:
    error = ConnectionError::Timeout;
    break;
  default:
    error = ConnectionError::UnknownError;
    break;
  };

  if (error.has_value())
    emit this->hasConnectionErrorOccured(error.value());
}

void GameServerController::connectRequestTimer(std::function<void()> slot) {
  requestCounter = 0;
  requestTimer->start(1000);
  connect(requestTimer, &QTimer::timeout, this, slot);
}

void GameServerController::disconnectRequestTimer(bool failed) {
  requestTimer->stop();
  requestCounter = 0;
  if (failed) {
    emit hasCommunicationStateChanged(CommunicationStatus::Failed);
  } else {
    emit hasCommunicationStateChanged(CommunicationStatus::Good);
  }
}