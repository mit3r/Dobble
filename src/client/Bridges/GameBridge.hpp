
#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class GameBridge : public QObject {
  Q_OBJECT
  public:
  explicit GameBridge(QObject* parent = nullptr) : QObject(parent) {}

  signals:  // signals emitted to ui

  void setPlayerStatus(
      const QString& nickname,
      const QString& status);

  void setPlayerScore(
      const QString& nickname,
      const int& score);

  void setTopCard(
      const int& cardId);

  void setPlayerCard(
      const QString& nickname,
      const int& cardId);

  void setLastsCards(
      const int& count);

  public slots:  // slots callable from ui
  void match(
      const int& pick1,
      const int& pick2);

  void quitGame();
};
