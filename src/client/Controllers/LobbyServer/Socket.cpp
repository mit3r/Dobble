#include "LobbyServerController.hpp"

LobbyServerController::LobbyServerController(QObject* parent) {
  Q_UNUSED(parent);
  socket = new QTcpSocket(this);
  commandFactory = LobbyCommandFactory(); // TODO: make it singleton?

  // Connect socket signals to slots
  connect(socket, &QTcpSocket::readyRead, this, &LobbyServerController::whenReadReady);
  connect(socket, &QTcpSocket::stateChanged, this, &LobbyServerController::whenSocketStateChanged);
  connect(socket, &QTcpSocket::errorOccurred, this, &LobbyServerController::whenSocketError);

  // Initialize request timer
  connect(requestTimer, &QTimer::timeout, [this]() {
    switch (requestCounter++) {
    case 1:
      emit hasCommunicationStateChanged(CommunicationStatus::Retrying);
      break;
    case 5:
      emit hasCommunicationStateChanged(CommunicationStatus::Failed);
      disconnectRequestTimer(true);
      wantDisconnect();
      break;
    }
  });
}

void LobbyServerController::wantConnectToServer(const std::string& ip, const int& port) {
  qDebug() << "LobbyServerController: Connecting to server at" << QString::fromStdString(ip) << ":"
           << port;

  socket->connectToHost(QString::fromStdString(ip), port, QIODevice::ReadWrite);
}

void LobbyServerController::whenReadReady() {
  // Append all available data to buffer
  receiveBuffer.append(socket->readAll());

  // Try to extract and process complete packets from buffer
  auto packetOpt = try_extract_packet(receiveBuffer);
  if (!packetOpt.has_value())
    return;

  json packet = packetOpt.value();

  if (!packet.contains("command")) {
    std::cout << "[CLIENT ERROR] Received packet without command field." << std::endl;
    return;
  }

  disconnectRequestTimer(false); // Successfully received a packet
  std::visit(*this, this->commandFactory.get(packet));
}

void LobbyServerController::whenSocketStateChanged(QTcpSocket::SocketState socketState) {
  qDebug() << "LobbyServerController: Socket state changed to" << static_cast<int>(socketState);

  std::optional<ConnectionStatus> connState;
  CommunicationStatus comState = CommunicationStatus::None;
  switch (socketState) {
  case QTcpSocket::ConnectedState:
    connState = ConnectionStatus::Connected;
    comState = CommunicationStatus::Good;
    break;
  case QTcpSocket::ConnectingState:
    connState = ConnectionStatus::Connecting;
    comState = CommunicationStatus::None;
    break;
  case QTcpSocket::UnconnectedState:
    connState = ConnectionStatus::Disconnected;
    break;
  case QTcpSocket::HostLookupState:
    connState = ConnectionStatus::HostLookup;
    comState = CommunicationStatus::None;
    break;
  default:
    break;
  };

  if (connState.has_value()) {
    emit this->hasConnectionStateChanged(connState.value());
    emit this->hasCommunicationStateChanged(comState);
  }
}

void LobbyServerController::whenSocketError(QTcpSocket::SocketError socketError) {
  qDebug() << "LobbyServerController: Socket error occurred:" << static_cast<int>(socketError);

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

void LobbyServerController::connectRequestTimer(std::function<void()> slot) {
  if (requestTimer->isActive())
    return;

  emit hasCommunicationStateChanged(CommunicationStatus::Waiting);

  connect(requestTimer, &QTimer::timeout, slot);
  requestTimer->start(500); // 0.5 second interval
  requestCounter = 0;
}

void LobbyServerController::disconnectRequestTimer(bool failed) {
  requestTimer->stop();
  requestCounter = 0;

  if (failed)
    emit hasCommunicationStateChanged(CommunicationStatus::Failed);
  else
    emit hasCommunicationStateChanged(CommunicationStatus::Good);
}