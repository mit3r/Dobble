

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
  void requestVerifyNickname(const std::string& nickname);
  void requestNavigateToPage(const double& page);
  void requestJoinGame(const std::string& gameId);
  void requestObserveGame(const std::string& gameId);

  // Bridge -> UI (js listeners), "on"
  void onServerConnectionStateChanged(const ConnectionStatus& status);
  void onServerConnectionErrorOccurred(const ConnectionError& error);

  void onServerCommunicationStateChanged(const CommunicationStatus& status);

  void onLoginSucceeded(const std::string& nickname);
  void onLoginFailed(const std::string& error);

  void onPageChanged(const int& pageNumber, const std::list<ShortGameInfo>& gamesList);

  // Bridge <- UI (js methods), "call"
private slots:
  void callVerifyNickname(const std::string& nickname);
  void callNavigateToPage(const double& page);
  void callJoinGame(const std::string& gameId);
  void callObserveGame(const std::string& gameId);
};