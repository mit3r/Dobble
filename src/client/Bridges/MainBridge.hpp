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
  public slots:
      void hasLobbyServerStateChanged(const ServerStatus& status);
      void hasGlobalErrorOccured(const std::string& error);

      // App <- Bridge (signals), "request"
  signals:
  void requestCloseApp();

  // Bridge -> UI (js listeners), "on"
  signals:
      void onNavigated(const View& page);
      void onLobbyServerStateChanged(const ServerStatus& status);

      void onGlobalErrorOccured(const QString& error);

      // Bridge <- UI (js methods), "call"
  private slots:
  void callCloseApp();
};