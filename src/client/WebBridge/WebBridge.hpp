
#include <QDebug>
#include <QObject>

#pragma once

class WebBridge : public QObject {
  Q_OBJECT
  public:
      explicit WebBridge(QObject* parent = nullptr) : QObject(parent) {}
  signals:
      /**
       * Signal emitted when page should change
       * @param page - One of: "start", "room", "game", "end"
       */
      void pageChanged(const QString& page);

      /**
       * Signal emitted when complete lobby information changes
       * @param lobbyName - Name of the lobby
       * @param players - List of player objects (QVariantMap with "name" key)
       * @param state - Lobby state: "waiting", "playing", or "finished"
       */
      void lobbyInfoChanged(const QString& lobbyName,
                            const QVariantList& players,
                            const QString& state);

      /**
       * Signal emitted when lobby should be cleared
       */
      void lobbyCleared();

      /**
       * [Optional] Signal for updating only players list
       * @param players - List of player objects (QVariantMap with "name" key)
       */
      void playersChanged(const QVariantList& players);

      /**
       * [Optional] Signal for updating only lobby state
       * @param state - Lobby state: "waiting", "playing", or "finished"
       */
      void lobbyStateChanged(const QString& state);

  public slots:
      // Jeśli chcesz wywołać metody z JavaScript, dodaj sloty tutaj
      // Przykład:
      // QString getCurrentPage() const;
      // void doSomething();

  private:
      // Twoje dane członkowskie
};

