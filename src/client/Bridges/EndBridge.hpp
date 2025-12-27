
#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class EndBridge : public QObject {
  Q_OBJECT
  public:
  explicit EndBridge(QObject* parent = nullptr) : QObject(parent) {}

  signals:  // Signals emmitted to ui
  void setScoreboard(
      const QList<QString>& names,
      const QList<int>& scores);

  public slots:  // Slots called from ui
  void returnToBrowse();
};
