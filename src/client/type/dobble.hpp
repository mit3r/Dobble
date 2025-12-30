#include <QString>
#include <QTcpSocket>
#include <string>

#pragma once

enum class View {
  Start = 0,
  Browser = 1,
  Room = 2,
  Game = 3,
  End = 4,
};

// Status of tcp connection to server
enum class ConnectionStatus {
  Connecting = 0,
  Connected = 1,
  Retrying = 2,
  Disconnected = 3,
};

// Error types of connection to server
enum class ConnectionError {
  None = 0,
  ConnectionRefused = 1,
  HostNotFound = 2,
  NetworkError = 3,
  Timeout = 4,
  UnknownError = 5,
};

// Status of communication with server
enum class CommunicationStatus {
  None = 0,          // before TCP connected
  Connected = 1,     // after TCP connected
  Waiting = 2,       // after request sent
  Retrying = 3,      // after ping-pong timeout
  Disconnected = 4,  // after several ping-pong timeouts
};

// enum class GameStatus {
//   Waiting = 0,
//   InGame = 1,
//   Finished = 2,
// };

// struct GameInfo {
//   std::string gameId;
// };