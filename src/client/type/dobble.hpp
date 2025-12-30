#include <QString>

#pragma once

enum class View {
  Start = 0,
  Browser = 1,
  Room = 2,
  Game = 3,
  End = 4,
};

enum class GameStatus {
  Waiting = 0,
  InGame = 1,
  Finished = 2,
};

struct GameInfo {
  QString gameId;
};