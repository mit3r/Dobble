#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class MainBridge : public QObject {
  Q_OBJECT
  public:
  explicit MainBridge(QObject* parent = nullptr) : QObject(parent) {}

  signals:  // Signals emmitted to ui
  void setPage(const QString& page);
  void setError(const QString& message);

  public slots:  // Slots called from ui
  void close();
};