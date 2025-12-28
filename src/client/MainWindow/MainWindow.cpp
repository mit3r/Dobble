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

  // Example: Connect lobby bridge signal to lobby server controller slot

  connect(
      ui->startBridge,
      &StartBridge::requestVerifyNickname,
      lobbyServerController,
      &LobbyServerController::wantNicknameVerification);
}

MainWindow::~MainWindow() {
}
