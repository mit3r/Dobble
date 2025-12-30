#include "MainBridge.hpp"

void MainBridge::hasNavigated(const QString& page) {
  qDebug() << "MainBridge: hasPageChanged called with page:" << page;
  emit onNavigated(page);
}

void MainBridge::callCloseApp() {
  qDebug() << "MainBridge: callCloseApp invoked from UI.";
  emit requestCloseApp();
}
