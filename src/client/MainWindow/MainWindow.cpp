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
      ui->startBridge, &StartBridge::requestVerifyNickname,
      lobbyServerController, &LobbyServerController::wantNicknameVerification);

  connect(
      lobbyServerController, &LobbyServerController::hasLoginSucceeded,
      ui->startBridge, &StartBridge::hasLoginSucceeded);
}

MainWindow::~MainWindow() {
}
