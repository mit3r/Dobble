#include <QDebug>
#include <QObject>
#include <QString>

#include "type/dobble.hpp"

#pragma once

/// @see ui/src/bridges/MainBridge.tsx
class MainBridge : public QObject {
  Q_OBJECT
  public:
  explicit MainBridge(QObject* parent = nullptr) : QObject(parent) {}

  // App -> Bridge (slots), "has"

  // App <- Bridge (signals), "request"

  // Bridge -> UI (js listeners), "on"
signals:
  void onNavigated(const View& page);
  void onGlobalErrorOccured(const QString& errorMessage);

  // Bridge <- UI (js methods), "call"
};