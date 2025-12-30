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
  if (!packetOpt.has_value()) return;

  json packet = packetOpt.value();

  if (!packet.contains("command")) {
    std::cout << "[CLIENT ERROR] Received packet without command field." << std::endl;
    return;
  }

  std::visit(*this, this->commandFactory.get(packet));
}

void LobbyServerController::whenSocketStateChanged(QTcpSocket::SocketState socketState) {
  emit this->hasConnectionStateChanged(static_cast<ConnectionStatus>(socketState));
}

void LobbyServerController::whenSocketError(QTcpSocket::SocketError socketError) {
  emit this->hasConnectionErrorOccurred(static_cast<ConnectionError>(socketError));
}