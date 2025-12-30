
#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QHostAddress>
#include <QMainWindow>
#include <QMimeDatabase>
#include <QObject>
#include <QTimer>
#include <QVBoxLayout>
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineView>
#include <iostream>

#include "Bridges/BrowserBridge.hpp"
#include "Bridges/GameBridge.hpp"
#include "Bridges/MainBridge.hpp"
#include "LocalsSchemeHandler.hpp"

#pragma once

class UIWidget : public QWidget {
  Q_OBJECT

  private:
  QVBoxLayout* layout;
  QWebEngineView* webView;
  QWebChannel* webChannel;

  public:
  // Bridges - UI communication
  MainBridge* mainBridge;  // Communicates on all pages

  // Specific page bridges
  BrowserBridge* browserBridge;
  GameBridge* gameBridge;

  UIWidget(QWidget* parent = nullptr);
  ~UIWidget();
};