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

  // Connect page flows
  connectLoginPageFlow();
  connectBrowserPageFlow();
  connectLobbyPageFlow();
  connectGamePageFlow();
  connectEndPageFlow();
}

MainWindow::~MainWindow() {
}

void MainWindow::connectLoginPageFlow() {
  connect(
      ui->loginBridge, &LoginBridge::requestVerifyNickname,
      lobbyServerController, &LobbyServerController::wantNicknameVerification);

  connect(
      lobbyServerController, &LobbyServerController::hasLoginSucceeded,
      ui->loginBridge, &LoginBridge::hasLoginSucceeded);

  connect(
      lobbyServerController, &LobbyServerController::hasLoginSucceeded,
      [this](const QString&) { emit ui->mainBridge->onNavigated(View::Browser); });

  connect(
      lobbyServerController, &LobbyServerController::hasLoginFailed,
      ui->loginBridge, &LoginBridge::hasLoginFailed);
}

void MainWindow::connectBrowserPageFlow() {
}

void MainWindow::connectLobbyPageFlow() {
}

void MainWindow::connectGamePageFlow() {
}

void MainWindow::connectEndPageFlow() {
}
