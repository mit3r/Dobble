

#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>

#pragma once

enum GameStatus {
  Waiting = 0,
  InGame = 1,
  Finished = 2
};

struct GameInfo {
  Q_GADGET
  Q_PROPERTY(QString gameId MEMBER gameId)
  Q_PROPERTY(QString gameName MEMBER gameName)
  Q_PROPERTY(int players MEMBER players)
  Q_PROPERTY(int maxPlayers MEMBER maxPlayers)
  Q_PROPERTY(QStringList nicknames MEMBER nicknames)
  Q_PROPERTY(GameStatus status MEMBER status)

  public:
  QString gameId;
  QString gameName;
  int players;
  int maxPlayers;
  QStringList nicknames;
  GameStatus status;
};

/**
 * @brief Controller for the browser page logic
 */
class BrowserBridge : public QObject {
  Q_OBJECT
  public:
  explicit BrowserBridge(QObject* parent = nullptr) : QObject(parent) {}

  signals:
  // signals emitted to ui
  void setPage(
      const int& pageNumber,
      const QList<GameInfo>& gamesList);

  void showErrorMessage(
      const QString& message);

  // signals emitted from ui
  void requestNavigateToPage(
      const double& page);
  void requestJoinGame(
      const QString& gameId);
  void requestObserveGame(
      const QString& gameId);

  public slots:  // slots callable from ui
  void navigateToPage(const double& page);
  void joinGame(const QString& gameId);
  void observeGame(const QString& gameId);
};