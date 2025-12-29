#include "StartBridge.hpp"

void StartBridge::hasLoginSucceeded(const QString& nickname) {
  qDebug() << "StartBridge: hasLoginSucceeded called with nickname:" << nickname;
  emit onLoginSucceeded(nickname);
}

void StartBridge::hasLoginFailed(const QString& error) {
  qDebug() << "StartBridge: hasLoginFailed called.";
  emit onLoginFailed(error);
}

void StartBridge::callVerifyNickname(const QString& nickname) {
  qDebug() << "StartBridge: callVerifyNickname called with nickname:" << nickname;
  emit requestVerifyNickname(nickname);
}
