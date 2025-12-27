#include "GameBridge.hpp"

void GameBridge::match(const int& pick1, const int& pick2) {
  qDebug() << "Matching picks:" << pick1 << "and" << pick2;
}

void GameBridge::quitGame() {
  qDebug() << "Quitting game.";
}
