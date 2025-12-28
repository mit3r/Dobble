#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class StartBridge : public QObject {
  Q_OBJECT
  public:
  explicit StartBridge(QObject* parent = nullptr) : QObject(parent) {}

  // App -> Bridge (slots), "has"
  public slots:
  void hasNicknameVerified();

  // App <- Bridge (signals), "request"
  signals:
  void requestVerifyNickname(const QString& nickname);

  // Bridge -> UI (js listeners), "on"
  signals:
  void onNicknameVerified();

  // Bridge <- UI (js methods), "call"
  public slots:
  void callVerifyNickname(const QString& nickname);
};