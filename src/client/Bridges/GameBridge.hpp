
#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class GameBridge : public QObject {
  Q_OBJECT
  public:
  explicit GameBridge(QObject* parent = nullptr) : QObject(parent) {}

  // App -> Bridge (slots), "has"
  public slots:
  void hasPlayerStatusChanged(const QString& nickname, const QString& status);
  void hasPlayerScoreChanged(const QString& nickname, const int& score);
  void hasTopCardChanged(const int& cardId);
  void hasPlayerCardChanged(const QString& nickname, const int& cardId);
  void hasLastsCardsChanged(const int& count);
  void hasQuitGame();

  // App <- Bridge (signals), "request"
  signals:
  void requestMatch(const int& pick1, const int& pick2);
  void requestQuitGame();

  // Bridge -> UI (js listeners), "on"
  signals:
  void onPlayerStatusChanged(const QString& nickname, const QString& status);
  void onPlayerScoreChanged(const QString& nickname, const int& score);
  void onTopCardChanged(const int& cardId);
  void onPlayerCardChanged(const QString& nickname, const int& cardId);
  void onLastsCardsChanged(const int& count);
  void onGameQuit();

  // Bridge <- UI (js methods), "call"
  private slots:
  void callMatch(const int& pick1, const int& pick2);
  void callQuitGame();
};
