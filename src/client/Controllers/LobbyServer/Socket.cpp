#include "LobbyControllerUtils.hpp"
#include "LobbyServerController.hpp"

LobbyServerController::LobbyServerController(QObject* parent) {
  Q_UNUSED(parent);
  socket = new QTcpSocket(this);
  commandFactory = LobbyCommandFactory();  // TODO: make it singleton?

  // Connect socket signals to slots
  connect(socket, &QTcpSocket::readyRead,
          this, &LobbyServerController::whenReadReady);

  connect(socket, &QTcpSocket::stateChanged,
          this, &LobbyServerController::whenSocketStateChanged);

  connect(socket, &QTcpSocket::errorOccurred,
          this, &LobbyServerController::whenSocketError);

  // Connect socket to server
  socket->connectToHost("127.0.0.1", 1500, QIODevice::ReadWrite);
}

void LobbyServerController::whenReadReady() {
  // Append all available data to buffer
  receiveBuffer.append(socket->readAll());

  // Try to extract and process complete packets from buffer
  auto packetOpt = try_extract_packet(receiveBuffer);
  if (!packetOpt.has_value()) return;

  json packet = packetOpt.value();

  if (!packet.contains("command")) {
    std::cout << "[CLIENT ERROR] Received packet without command field." << std::endl;
    return;
  }

  std::visit(*this, this->commandFactory.get(packet));
}

void LobbyServerController::whenSocketStateChanged(QTcpSocket::SocketState socketState) {
  // Communitaction two states

  // TCP Socket states:
  // QTcpSocket::SocketState::ConnectedState;
  // QTcpSocket::SocketState::ConnectingState;
  // QTcpSocket::SocketState::HostLookupState;
  // QTcpSocket::SocketState::UnconnectedState;

  switch (socketState) {
  case QTcpSocket::SocketState::ConnectedState:
    emit hasLoginSucceeded(currentNickname.value_or("Unknown"));
    break;
  case QTcpSocket::SocketState::UnconnectedState:
    emit hasDisconnected();
    break;
  default:
    break;
  }
}

void LobbyServerController::whenSocketError(QTcpSocket::SocketError socketError) {
  // Communitaction errors

  // TCP Socket errors:
  // QTcpSocket::SocketError::ConnectionRefusedError;
  // QTcpSocket::SocketError::HostNotFoundError;
  // QTcpSocket::SocketError::NetworkError;
  // QTcpSocket::SocketError::OperationError;
  // QTcpSocket::SocketError::RemoteHostClosedError;
  // QTcpSocket::SocketError::SocketAccessError;
  // QTcpSocket::SocketError::SocketResourceError;
  // QTcpSocket::SocketError::SocketTimeoutError;
  // QTcpSocket::SocketError::TemporaryError;
  // QTcpSocket::SocketError::UnknownSocketError;

  switch (socketError) {
  case QTcpSocket::SocketError::ConnectionRefusedError:
    emit hasErrorOccurred("Connection refused by the server.");
    break;
  case QTcpSocket::SocketError::HostNotFoundError:
    emit hasErrorOccurred("Host not found. Check the server address.");
    break;
  case QTcpSocket::SocketError::NetworkError:
    emit hasErrorOccurred("Network error occurred.");
    break;
  case QTcpSocket::SocketError::RemoteHostClosedError:
    emit hasDisconnected();
    break;
  default:
    emit hasErrorOccurred("An unknown socket error occurred.");
    break;
  }
}