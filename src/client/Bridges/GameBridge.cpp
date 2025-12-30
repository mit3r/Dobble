#include "GameBridge.hpp"

void GameBridge::callMatch(const int& pick1, const int& pick2) {
  qDebug() << "GameBridge::callMatch called with"
           << pick1 << pick2;
  emit requestMatch(pick1, pick2);
}

void GameBridge::callQuitGame() {
  qDebug() << "GameBridge::callQuitGame called";
  emit requestQuitGame();
}
