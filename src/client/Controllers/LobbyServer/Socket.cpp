#include "LobbyControllerUtils.hpp"
#include "LobbyServerController.hpp"

LobbyServerController::LobbyServerController(QObject* parent) {
  Q_UNUSED(parent);
  socket = new QTcpSocket(this);
  commandFactory = LobbyCommandFactory();  // TODO: make it singleton?

  // Connect socket signals to slots
  connect(socket, &QTcpSocket::readyRead, this, &LobbyServerController::whenReadReady);

  // Connect socket to server
  socket->connectToHost("127.0.0.1", 1500, QIODevice::ReadWrite);
}

void LobbyServerController::whenReadReady() {
  // Append all available data to buffer
  receiveBuffer.append(socket->readAll());

  // Try to extract and process complete packets from buffer
  while (true) {
    auto packetOpt = try_extract_packet(receiveBuffer);

    if (!packetOpt.has_value()) {
      // No complete packet available yet
      break;
    }

    json packet = packetOpt.value();

    if (!packet.contains("command")) {
      std::cerr << "[CLIENT ERROR] Received packet without command field." << std::endl;
      continue;
    }

    std::cout << "[CLIENT] Received command: " << packet["command"] << std::endl;

    // Visit the command variant to handle it
    std::visit(*this, this->commandFactory.get(packet));
  }
}
