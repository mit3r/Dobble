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
  connectGameFlow();

  // Application start flow
  connect(ui->mainBridge, &MainBridge::uiIsReady, this, [this]() {
    qDebug() << "UI is ready.";
    lobbyServerController->wantConnectToServer("127.0.0.1", 1500);
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

void MainWindow::connectLoginPageFlow() {
  // Nickname verification request
  connect(ui->browserBridge, &BrowserBridge::requestVerifyNickname,
          [this](const std::string& nickname) {
            this->nickname = nickname;
            lobbyServerController->wantNicknameVerification(nickname);
          });

  //   Login success handling
  connect(lobbyServerController, &LobbyServerController::hasLoginSucceeded,
          [this](const std::string& clientId) {
            this->clientId = clientId;

            emit ui->browserBridge->onLoginSucceeded(
                QString::fromStdString(this->nickname.value()));
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
            this->clientId.reset();
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

void MainWindow::connectGameServerFlow() {

  // Connect to game request
  connect(
      ui->gameBridge, &GameBridge::requestJoinToGame,
      [this](const std::string& ip, const int& port, const std::string& gameId, const Role& role) {
        if (!this->clientId.has_value())
          return emit this->gameServerController->hasConnectGameFailed(
              "Client ID is not set! Cannot join game.");

        if (!this->nickname.has_value())
          return emit this->gameServerController->hasConnectGameFailed(
              "Nickname is not set! Cannot join game.");

        gameServerController->wantConnectToGame(ip, port, gameId, this->clientId.value(),
                                                this->nickname.value(), role);
      });

  // Handle connection result
  connect(gameServerController, &GameServerController::hasConnectGameSucceed,
          [this]() { emit ui->mainBridge->onNavigated(View::Room); });

  connect(gameServerController, &GameServerController::hasConnectGameFailed,
          [this](const QString& error) {
            emit ui->mainBridge->onNavigated(View::Browser);
            emit ui->mainBridge->onGlobalErrorOccured(error);
          });

  // Handle server communication signals
  connect(gameServerController, &GameServerController::hasCommunicationStateChanged,
          [this](const CommunicationStatus& status) {
            emit ui->gameBridge->onServerCommunicationStateChanged(status);
          });

  connect(gameServerController, &GameServerController::hasConnectionStateChanged,
          [this](const ConnectionStatus& status) {
            emit ui->gameBridge->onServerConnectionStateChanged(status);
          });

  connect(gameServerController, &GameServerController::hasConnectionErrorOccured,
          [this](const ConnectionError& error) {
            emit ui->gameBridge->onServerConnectionErrorOccured(error);
          });

  connect(gameServerController, &GameServerController::hasConnectionStateChanged,
          [this](const ConnectionStatus& status) {
            if (status == ConnectionStatus::Disconnected)
              emit ui->mainBridge->onNavigated(View::Browser);
          });
}

void MainWindow::connectGameFlow() {
  // Joining game flow
  connect(gameServerController, &GameServerController::hasConnectGameSucceed,
          [this]() { emit ui->mainBridge->onNavigated(View::Room); });

  connect(gameServerController, &GameServerController::hasConnectGameFailed,
          [this](const QString& error) {
            emit ui->mainBridge->onNavigated(View::Browser);
            emit ui->mainBridge->onGlobalErrorOccured(error);
          });

  //  Game start flow
  connect(ui->gameBridge, &GameBridge::requestStartGame, gameServerController,
          &GameServerController::wantStartGame);

  connect(gameServerController, &GameServerController::hasGameStartedSucceed,
          [this]() { emit ui->mainBridge->onNavigated(View::Game); });

  connect(gameServerController, &GameServerController::hasGameStartedFailed,
          [this](const QString& error) {
            emit ui->mainBridge->onNavigated(View::End);
            emit ui->mainBridge->onGlobalErrorOccured(error);
          });

  // Match cards
  connect(ui->gameBridge, &GameBridge::requestMatch, gameServerController,
          &GameServerController::wantMatchCard);

  connect(gameServerController, &GameServerController::hasMatchResult, ui->gameBridge,
          &GameBridge::onMatchResult);

  // Game info updates
  connect(gameServerController, &GameServerController::hasGameInfoUpdated, ui->gameBridge,
          &GameBridge::onGameInfoChanged);

  // Quit game flow
  connect(ui->gameBridge, &GameBridge::requestQuitGame, gameServerController,
          &GameServerController::wantLeaveGame);
}
