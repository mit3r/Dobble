#include <QString>
#include <QTcpSocket>
#include <string>

#pragma once

enum class View {
  Login,
  Browser,
  Room,
  Game,
  End,
};

// Status of tcp connection to server
enum class ConnectionStatus {
  Connecting,
  HostLookup,
  Connected,
  Retrying,
  Disconnected,
};

// Error types of connection to server
enum class ConnectionError {
  None,
  ConnectionRefused,
  HostNotFound,
  NetworkError,
  Timeout,
  UnknownError,
};

// Status of communication with server
enum class CommunicationStatus {
  None,     // before TCP connected
  Good,     // after TCP connected
  Waiting,  // after request sent
  Retrying, // after ping-pong timeout
  Failed,   // after several ping-pong timeouts
};

// Browser and Game screen
enum class GameStatus {
  Waiting,
  InGame,
  Finished,
};

struct CShortGameInfo {
  QString gameId;
  QString gameName;
  QString ip;
  int port;
  int players;
  int maxPlayers;
  GameStatus status;

  bool operator==(const CShortGameInfo& other) const {
    return gameId == other.gameId && gameName == other.gameName && players == other.players &&
           maxPlayers == other.maxPlayers && status == other.status && ip == other.ip &&
           port == other.port;
  }

  QVariantMap toVariantMap() const {
    QVariantMap map;
    map["gameId"] = gameId;
    map["gameName"] = gameName;
    map["ip"] = ip;
    map["port"] = port;
    map["players"] = players;
    map["maxPlayers"] = maxPlayers;
    map["status"] = static_cast<int>(status);
    return map;
  }
};

enum class Role {
  Player,
  Observer,
};

enum class MatchResult {
  Correct,
  Incorrect,
  TooLate,
};

struct CPlayerGameInfo {

  QString nickname;
  int cardId;
  int points;   // matches - mistakes
  int matches;  // total correct matches
  int mistakes; // total mistakes

  bool operator==(const CPlayerGameInfo& other) const {
    return nickname == other.nickname && cardId == other.cardId && points == other.points &&
           matches == other.matches && mistakes == other.mistakes;
  }

  QVariantMap toVariantMap() const {
    QVariantMap map;
    map["nickname"] = nickname;
    map["cardId"] = cardId;
    map["points"] = points;
    map["matches"] = matches;
    map["mistakes"] = mistakes;
    return map;
  }
};

struct CGameInfo {
  QString gameId;
  QString turnId;
  QString winnerNickname;
  QList<CPlayerGameInfo> players;
  GameStatus status;

  int topCardId;
  int lastsCards;

  QVariantMap toVariantMap() const {
    QVariantMap map;
    map["gameId"] = gameId;
    map["turnId"] = turnId;
    map["winnerNickname"] = winnerNickname;

    QVariantList playersList;
    for (const auto& player : players) {
      playersList.append(player.toVariantMap());
    }
    map["players"] = playersList;

    map["status"] = static_cast<int>(status);
    map["topCardId"] = topCardId;
    map["lastsCards"] = lastsCards;
    return map;
  }
};