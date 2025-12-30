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

  // Connect controllers' flows
  connectLobbyServerFlow();
  connectGameServerFlow();

  // Connect page flows
  connectLoginPageFlow();
  connectBrowserPageFlow();
  connectLobbyPageFlow();
  connectGamePageFlow();
  connectEndPageFlow();
}

MainWindow::~MainWindow() {
}

void MainWindow::connectLobbyServerFlow() {
  connect(lobbyServerController, &LobbyServerController::hasCommunicationStateChanged,
          [this](const CommunicationStatus& status) {
            emit ui->browserBridge->onServerCommunicationStateChanged(status);
          });

  connect(lobbyServerController, &LobbyServerController::hasConnectionStateChanged,
          [this](const ConnectionStatus& status) {
            emit ui->browserBridge->onServerConnectionStateChanged(status);
          });

  connect(lobbyServerController, &LobbyServerController::hasConnectionErrorOccurred,
          [this](const ConnectionError& error) {
            emit ui->browserBridge->onServerConnectionErrorOccurred(error);
          });
}

void MainWindow::connectGameServerFlow() {}

void MainWindow::connectLoginPageFlow() {
  // Nickname verification request
  connect(ui->browserBridge, &BrowserBridge::requestVerifyNickname, lobbyServerController,
          &LobbyServerController::wantNicknameVerification);

  //   Login success handling
  connect(lobbyServerController, &LobbyServerController::hasLoginSucceeded,
          [this](const std::string& nickname) {
            emit ui->browserBridge->onLoginSucceeded(nickname);
            emit ui->mainBridge->onNavigated(View::Browser);
          });

  //   Login failure handling
  connect(lobbyServerController, &LobbyServerController::hasLoginFailed,
          [this](const std::string& error) { emit ui->browserBridge->onLoginFailed(error); });
}

void MainWindow::connectBrowserPageFlow() {}

void MainWindow::connectLobbyPageFlow() {
}

void MainWindow::connectGamePageFlow() {
}

void MainWindow::connectEndPageFlow() {
}
