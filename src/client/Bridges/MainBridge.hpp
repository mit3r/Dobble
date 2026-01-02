#include <QDebug>
#include <QObject>
#include <QString>

#include "type/dobble.hpp"

#pragma once

/**
 * @brief Controller for communication between app and ui, related to main application actions:
 * - navigation,
 * - global error handling
 */
class MainBridge : public QObject {
  Q_OBJECT
  public:
  explicit MainBridge(QObject* parent = nullptr) : QObject(parent) {}

signals:
  // App <- Bridge (signals), "request"

  // Bridge -> UI (js listeners), "on"
  void onNavigated(const View& page);
  void onGlobalErrorOccured(const QString& errorMessage);

  // Bridge <- UI (js methods), "call"
};