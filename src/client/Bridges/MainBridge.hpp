#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class MainBridge : public QObject {
  Q_OBJECT
  public:
  explicit MainBridge(QObject* parent = nullptr) : QObject(parent) {}

  // App -> Bridge (slots), "has"
  public slots:
      void hasNavigated(const QString& page);

      // App <- Bridge (signals), "request"
  signals:
  void requestCloseApp();

  // Bridge -> UI (js listeners), "on"
  signals:
      void onNavigated(const QString& page);

      // Bridge <- UI (js methods), "call"
  private slots:
  void callCloseApp();
};