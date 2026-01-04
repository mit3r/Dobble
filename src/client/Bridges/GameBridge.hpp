
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
  void requestJoinToGame(const std::string& ip, const int& port, const std::string& gameId,
                         const Role& role);

  void requestStartGame();
  void requestMatch(const std::string& turnId, const int& symbolId);
  void requestQuitGame();

  // Bridge -> UI (js listeners), "on"
  void onServerConnectionStateChanged(const ConnectionStatus& status);
  void onServerConnectionErrorOccured(const ConnectionError& error);
  void onServerCommunicationStateChanged(const CommunicationStatus& status);

  void onGameInfoChanged(const QVariantMap& gameInfo);
  void onMatchResult(const MatchResult& result);
  void onGameQuit();

  // Bridge <- UI (js methods), "call"
public slots:
  void callJoinGame(const QString& ip, const int& port, const QString& gameId, const Role& role);
  void callStartGame();
  void callMatch(const QString& turnId, const int& symbolId);
  void callQuitGame();
};
