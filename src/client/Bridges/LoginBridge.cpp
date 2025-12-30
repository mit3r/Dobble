#include "LoginBridge.hpp"

void LoginBridge::hasLoginSucceeded(const QString& nickname) {
  qDebug() << "LoginBridge: hasLoginSucceeded called with nickname:" << nickname;
  emit onLoginSucceeded(nickname);
}

void LoginBridge::hasLoginFailed(const QString& error) {
  qDebug() << "LoginBridge: hasLoginFailed called with error:" << error;
  emit onLoginFailed(error);
}

void LoginBridge::callVerifyNickname(const QString& nickname) {
  qDebug() << "LoginBridge: callVerifyNickname called with nickname:" << nickname;
  emit requestVerifyNickname(nickname);
}
