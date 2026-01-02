
#include <QDebug>
#include <QObject>
#include <QString>

#include <protocol/lobby/CommonProtocolStructs.hpp>

#include "type/dobble.hpp"

#pragma once

/**
 * @brief Controller for communication between app and ui, related to in-game actions:
 * - fetch game info,
 * - match,
 * - quit game
 */
class GameBridge : public QObject {
  Q_OBJECT
  public:
  explicit GameBridge(QObject* parent = nullptr) : QObject(parent) {}

signals:
  // App <- Bridge (signals), "request"
  void requestMatch(const int& pick1, const int& pick2);
  void requestQuitGame();

  // Bridge -> UI (js listeners), "on"
  void onGameInfoChanged(const GameStruct& gameInfo);
  void onMatchResult(const bool& isMatch);
  void onGameQuit();

  // Bridge <- UI (js methods), "call"
private slots:
  void callMatch(const int& pick1, const int& pick2);
  void callQuitGame();
};
