#include "EndBridge.hpp"

void EndBridge::hasScoreboardChanged(
    const QList<QString>& names,
    const QList<int>& scores) {
  qDebug() << "EndBridge::hasScoreboardChanged called";
  emit onScoreboardChanged(names, scores);
}

void EndBridge::callReturnToBrowse() {
  qDebug() << "EndBridge::callReturnToBrowse called";
  emit requestReturnToBrowse();
}
