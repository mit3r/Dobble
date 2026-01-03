#include "MainBridge.hpp"

void MainBridge::callMsg(const QString& msg) {
  qDebug() << "\033[34m[MainBridge]\033[0m" << msg;
}

void MainBridge::callUIReady() {
  emit uiIsReady();
}
