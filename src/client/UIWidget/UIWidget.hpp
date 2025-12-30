
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
#include "Bridges/EndBridge.hpp"
#include "Bridges/GameBridge.hpp"
#include "Bridges/LobbyBridge.hpp"
#include "Bridges/LoginBridge.hpp"
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
  LoginBridge* loginBridge;
  BrowserBridge* browserBridge;
  LobbyBridge* lobbyBridge;
  GameBridge* gameBridge;
  EndBridge* endBridge;

  UIWidget(QWidget* parent = nullptr);
  ~UIWidget();
};