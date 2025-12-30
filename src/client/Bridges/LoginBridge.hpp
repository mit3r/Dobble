#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class LoginBridge : public QObject {
  Q_OBJECT
  public:
  explicit LoginBridge(QObject* parent = nullptr) : QObject(parent) {}

  // App -> Bridge (slots), "has"
  public slots:
  void hasLoginSucceeded(const QString& nickname);
  void hasLoginFailed(const QString& error);

  // App <- Bridge (signals), "request"
  signals:
  void requestVerifyNickname(const QString& nickname);

  // Bridge -> UI (js listeners), "on"
  signals:
  void onLoginSucceeded(const QString& nickname);
  void onLoginFailed(const QString& error);

  // Bridge <- UI (js methods), "call"
  public slots:
  void callVerifyNickname(const QString& nickname);
};