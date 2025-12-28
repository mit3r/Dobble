#include "MainBridge.hpp"

void MainBridge::hasPageChanged(const QString& page) {
  qDebug() << "MainBridge: hasPageChanged called with page:" << page;
  emit onPageChanged(page);
}

void MainBridge::callCloseApp() {
  qDebug() << "MainBridge: callCloseApp invoked from UI.";
  emit requestCloseApp();
}
