#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QHostAddress>
#include <QMainWindow>
#include <QMimeDatabase>
#include <QTimer>
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineView>
#include <iostream>

#include "LocalsSchemeHandler.hpp"
#include "WebBridge/WebBridge.hpp"

#pragma once

class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  private:
  QWebEngineView* webView;
  QWebChannel* webChannel;
  WebBridge* webBridge;
};
