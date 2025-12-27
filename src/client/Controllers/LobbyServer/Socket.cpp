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
  auto packetOpt = receive_json_packet(socket->socketDescriptor());
  if (!packetOpt.has_value()) {
    std::cerr << "[CLIENT ERROR] Failed to receive packet from lobby server." << std::endl;
    return;
  }

  json packet = packetOpt.value();
  if (!packet.contains("command")) {
    std::cerr << "[CLIENT ERROR] Received packet without command field." << std::endl;
    return;
  }

  std::cout << "[CLIENT] Received command: " << packet["command"] << std::endl;

  // Visit the command variant to handle it
  std::visit(*this, this->commandFactory.get(packet));
}
