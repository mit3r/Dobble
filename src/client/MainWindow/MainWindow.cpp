#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
  // Initialize window
  ui = new UIWidget(this);
  setCentralWidget(ui);
  resize(1024, 768);

  // Initialize controllers
  gameServerController = new GameServerController(this);
  lobbyServerController = new LobbyServerController(this);

  // Login verification flow
  connect(
      ui->loginBridge, &LoginBridge::requestVerifyNickname,
      lobbyServerController, &LobbyServerController::wantNicknameVerification);

  connect(
      lobbyServerController, &LobbyServerController::hasLoginSucceeded,
      ui->loginBridge, &LoginBridge::hasLoginSucceeded);

  connect(
      lobbyServerController, &LobbyServerController::hasLoginSucceeded,
      [this](const QString&) { emit ui->mainBridge->onNavigated("browser"); });

  connect(
      lobbyServerController, &LobbyServerController::hasLoginFailed,
      ui->loginBridge, &LoginBridge::hasLoginFailed);
}

MainWindow::~MainWindow() {
}
