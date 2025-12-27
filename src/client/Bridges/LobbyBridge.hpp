
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

  signals:  // signals emitted to ui
  void setLobbyName(
      const QString& lobbyName);
  void setPlayerList(
      const QStringList& playerList);
  void setLobbyStatus(
      const GameStatus& status);

  public slots:  // slots callable from ui
  void setReadyState(
      bool isReady);
  void quitLobby();
};