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

#include "Bridges/BrowserBridge.hpp"
#include "Bridges/GameBridge.hpp"
#include "Bridges/MainBridge.hpp"
#include "Controllers/GameServer/GameServerController.hpp"
#include "Controllers/LobbyServer/LobbyServerController.hpp"
#include "UIWidget/UIWidget.hpp"

#pragma once

/**
 * @brief Main application window containing the web view and managing communication
 *        between the UI and the backend controllers via bridges.
 */
class MainWindow : public QMainWindow {
  Q_OBJECT

  public:
  MainWindow(QWidget* parent = nullptr);
  ~MainWindow();

  private:
      UIWidget* ui;  // Manages the web view and UI bridges

      // Controllers - Network communication
      GameServerController* gameServerController;    // Communicates with game server
      LobbyServerController* lobbyServerController;  // Communicates with lobby server

      void connectLobbyServerFlow();
      void connectGameServerFlow();

      void connectLoginPageFlow();
      void connectBrowserPageFlow();
      void connectGameFlow();

    private:
      // Global state
      std::optional<std::string> clientId = std::nullopt;
      std::optional<std::string> nickname = std::nullopt;

      // Browser page state
      std::optional<int> page = 1;

      // Game state
      std::optional<Role> role = std::nullopt;
      std::optional<std::string> gameId = std::nullopt;
};
