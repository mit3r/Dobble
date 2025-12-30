
#include <QDebug>
#include <QObject>
#include <QString>

#include "type/dobble.hpp"

#pragma once

class GameBridge : public QObject {
  Q_OBJECT
  public:
  explicit GameBridge(QObject* parent = nullptr) : QObject(parent) {}

signals:
  // App <- Bridge (signals), "request"
  void requestMatch(const int& pick1, const int& pick2);
  void requestQuitGame();

  // Bridge -> UI (js listeners), "on"
  void onGameInfoChanged(const GameInfo& gameInfo);
  void onMatchResult(const bool& isMatch);
  void onGameQuit();

  // Bridge <- UI (js methods), "call"
private slots:
  void callMatch(const int& pick1, const int& pick2);
  void callQuitGame();
};
