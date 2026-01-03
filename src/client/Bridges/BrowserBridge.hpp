#include <QObject>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <type/dobble.hpp>
#include <protocol/lobby/CommonProtocolStructs.hpp>

#pragma once

/**
 * @brief Controller for communication between app and ui, related to lobby server actions:
 * - login,
 * - browse games,
 * - join/observe game
 */
class BrowserBridge : public QObject {
  Q_OBJECT
public:
  explicit BrowserBridge(QObject* parent = nullptr) : QObject(parent) {}

signals:
  // App <- Bridge (signals), "request"
  void requestConnectToLobbyServer(const std::string& ip, const int& port);
  void requestVerifyNickname(const std::string& nickname);
  void requestNavigateToPage(const int& page);
  void requestCreateGame(const std::string& gameName, const int& maxPlayers);
  void requestJoinGame(const std::string& gameId);
  void requestObserveGame(const std::string& gameId);

  // Bridge -> UI (js listeners), "on"
  void onServerConnectionStateChanged(const ConnectionStatus& status);
  void onServerConnectionErrorOccured(const ConnectionError& error);

  void onServerCommunicationStateChanged(const CommunicationStatus& status);

  void onLoginSucceeded(const QString& nickname);
  void onLoginFailed(const QString& error);

  void onPageChanged(const QVariantList& gamesList, const int& currentPage, const int& nextPage);

  // Bridge <- UI (js methods), "call"
public slots:
  void callConnectToLobbyServer(const QString& ip, const int& port);
  void callVerifyNickname(const QString& nickname);
  void callNavigateToPage(const int& page);
  void callJoinGame(const QString& gameId);
  void callObserveGame(const QString& gameId);
  void callCreateGame(const QString& gameName, const int& maxPlayers);
};