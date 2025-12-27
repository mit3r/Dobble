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

  // TODO: Add wantNicknameVerification slot to LobbyServerController class
  // connect(
  //     ui->startBridge, &StartBridge::requestNickname,
  //     lobbyServerController, &LobbyServerController::wantNicknameVerification);
}

MainWindow::~MainWindow() {
}
