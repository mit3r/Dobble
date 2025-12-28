
#include <QDebug>
#include <QObject>
#include <QString>
#include <QStringList>

#include "BrowserBridge.hpp"

#pragma once

class LobbyBridge : public QObject {
  Q_OBJECT
  public:
  explicit LobbyBridge(QObject* parent = nullptr) : QObject(parent) {}

  // App -> Bridge (slots), "has"
  public slots:
  void hasLobbyNameChanged(const QString& lobbyName);
  void hasPlayerListChanged(const QStringList& playerList);
  void hasLobbyStatusChanged(const GameStatus& status);
  void hasSetReadyState(bool isReady);
  void hasQuitLobby();

  // App <- Bridge (signals), "request"
  signals:
  void requestSetReadyState(bool isReady);

  // Bridge -> UI (js listeners), "on"
  signals:
  void onLobbyNameChanged(const QString& lobbyName);
  void onPlayerListChanged(const QStringList& playerList);
  void onLobbyStatusChanged(const GameStatus& status);
  void onReadyStateChanged(bool isReady);
  void onLobbyQuit();

  // Bridge <- UI (js methods), "call"
  private slots:
  void callSetReadyState(bool isReady);
  void callQuitLobby();
};