#include <QDebug>
#include <QObject>
#include <QString>

#pragma once

class StartBridge : public QObject {
  Q_OBJECT
  public:
  explicit StartBridge(QObject* parent = nullptr) : QObject(parent) {}

  signals:  // signals emitted to ui
  void showErrorMessage(
      const QString& message);

  public slots:  // slots callable from ui
  void requestNickname(const QString& nickname);
};
