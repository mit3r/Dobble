#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
  // Initialize window
  ui = new UIWidget(this);
  setCentralWidget(ui);
  resize(1024, 768);

  // Initialize controllers
  lobbyServerController = new LobbyServerController(this);
  gameServerController = new GameServerController(this);

  // Connect controllers' flows
  connectLobbyServerFlow();
  connectGameServerFlow();

  // Connect page flows
  connectLoginPageFlow();
  connectBrowserPageFlow();
  connectLobbyPageFlow();
  connectGamePageFlow();
  connectEndPageFlow();

  // Application start flow
  connect(ui->mainBridge, &MainBridge::uiIsReady, this, [this]() {
    qDebug() << "UI is ready.";
    lobbyServerController->wantConnectToServer("localhost", 1500);
  });
}

MainWindow::~MainWindow() {
}

void MainWindow::connectLobbyServerFlow() {

  connect(ui->browserBridge, &BrowserBridge::requestConnectToLobbyServer,
          [this](const std::string& ip, const int& port) {
            lobbyServerController->wantConnectToServer(ip, port);
          });

  connect(lobbyServerController, &LobbyServerController::hasCommunicationStateChanged,
          [this](const CommunicationStatus& status) {
            emit ui->browserBridge->onServerCommunicationStateChanged(status);
          });

  connect(lobbyServerController, &LobbyServerController::hasConnectionStateChanged,
          [this](const ConnectionStatus& status) {
            emit ui->browserBridge->onServerConnectionStateChanged(status);
          });

  connect(lobbyServerController, &LobbyServerController::hasConnectionErrorOccured,
          [this](const ConnectionError& error) {
            emit ui->browserBridge->onServerConnectionErrorOccured(error);
          });
}

void MainWindow::connectGameServerFlow() {}

void MainWindow::connectLoginPageFlow() {
  // Nickname verification request
  connect(ui->browserBridge, &BrowserBridge::requestVerifyNickname,
          [this](const std::string& nickname) {
            this->nickname = nickname;
            lobbyServerController->wantNicknameVerification(nickname);
          });

  //   Login success handling
  connect(lobbyServerController, &LobbyServerController::hasLoginSucceeded, [this]() {
    emit ui->browserBridge->onLoginSucceeded(QString::fromStdString(this->nickname.value()));
    emit ui->mainBridge->onNavigated(View::Browser);
  });

  //   Already logged in handling
  connect(lobbyServerController, &LobbyServerController::hasAlreadyLoggedIn, [this]() {
    emit ui->browserBridge->onLoginSucceeded(QString::fromStdString(this->nickname.value()));
    emit ui->mainBridge->onNavigated(View::Browser);
  });

  //   Login failure handling
  connect(lobbyServerController, &LobbyServerController::hasLoginFailed,
          [this](const std::string& error) {
            this->nickname.reset();
            emit ui->browserBridge->onLoginFailed(QString::fromStdString(error));
          });
}

void MainWindow::connectBrowserPageFlow() {

  // Create game request
  connect(ui->browserBridge, &BrowserBridge::requestCreateGame, lobbyServerController,
          &LobbyServerController::wantCreateGame);

  // Page navigation request
  connect(ui->browserBridge, &BrowserBridge::requestNavigateToPage, [this](const int& page) {
    this->page = page;
    lobbyServerController->wantNavigateToPage(page);
  });

  //  Page data received handling
  connect(lobbyServerController, &LobbyServerController::hasReceivedPage,
          [this](const QVariantList& games, const int& nextPage) {
            emit ui->browserBridge->onPageChanged(games, this->page.value_or(1), nextPage);
          });
}

void MainWindow::connectLobbyPageFlow() {
}

void MainWindow::connectGamePageFlow() {
}

void MainWindow::connectEndPageFlow() {
}
