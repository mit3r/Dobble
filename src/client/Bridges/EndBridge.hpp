
#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class EndBridge : public QObject {
  Q_OBJECT
  public:
  explicit EndBridge(QObject* parent = nullptr) : QObject(parent) {}

  // App -> Bridge (slots), "has"
  public slots:
  void hasScoreboardChanged(
      const QList<QString>& names,
      const QList<int>& scores);

  // App <- Bridge (signals), "request"
  signals:
  void requestReturnToBrowse();

  // Bridge -> UI (js listeners), "on"
  signals:
  void onScoreboardChanged(
      const QList<QString>& names,
      const QList<int>& scores);

  // Bridge <- UI (js methods), "call"
  private slots:
  void callReturnToBrowse();
};
