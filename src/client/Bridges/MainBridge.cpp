#include "MainBridge.hpp"

void MainBridge::callCloseApp() {
  qDebug() << "MainBridge: callCloseApp invoked";
  emit requestCloseApp();
}

void MainBridge::hasLobbyServerDisconnected() {
  qDebug() << "MainBridge: hasLobbyServerDisconnected invoked";
  emit onLobbyServerDisconnected();
}

void MainBridge::hasGameServerConnected() {
  qDebug() << "MainBridge: hasGameServerConnected invoked";
  emit onGameServerConnected();
}

void MainBridge::hasGameServerDisconnected() {
  qDebug() << "MainBridge: hasGameServerDisconnected invoked";
  emit onGameServerDisconnected();
}

void MainBridge::hasGlobalErrorOccured(const std::string& error) {
  qDebug() << "MainBridge: hasGlobalErrorOccured invoked with error:" << QString::fromStdString(error);
  emit onGlobalErrorOccured(QString::fromStdString(error));
}
