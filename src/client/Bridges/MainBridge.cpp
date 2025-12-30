#include "MainBridge.hpp"

void MainBridge::callCloseApp() {
  qDebug() << "MainBridge: callCloseApp invoked";
  emit requestCloseApp();
}