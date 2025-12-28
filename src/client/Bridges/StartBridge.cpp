#include "StartBridge.hpp"

void StartBridge::hasNicknameVerified() {
  qDebug() << "StartBridge: hasNicknameVerified called.";
  emit onNicknameVerified();
}

void StartBridge::callVerifyNickname(const QString& nickname) {
  qDebug() << "StartBridge: callVerifyNickname called with nickname:" << nickname;
  emit requestVerifyNickname(nickname);
}
