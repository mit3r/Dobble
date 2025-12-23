
#include <QDebug>
#include <QObject>

#pragma once

class WebBridge : public QObject {
  Q_OBJECT
  public:
  explicit WebBridge(QObject* parent = nullptr) : QObject(parent) {}

  // Function callable from JavaScript
  Q_INVOKABLE void receiveFromJs(const QString& message) {
    qDebug() << "Received from JS:" << message;
    // Reply back to JS
    emit sendToJs("Hello from C++! I received: " + message);
  }

  signals:
  // Signal to trigger a function in JavaScript
  void sendToJs(const QString& message);
};
