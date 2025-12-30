export enum View {
  Login = 0,
  Browser = 1,
  Room = 2,
  Game = 3,
  End = 4,
}

export enum ConnectionStatus {
  Connecting = 0,
  Connected = 1,
  Retrying = 2,
  Disconnected = 3,
};

export enum ConnectionError {
  None = 0,
  ConnectionRefused = 1,
  HostNotFound = 2,
  NetworkError = 3,
  Timeout = 4,
  UnknownError = 5,
};

export enum CommunicationStatus {
  None = 0,          // before TCP connected
  Connected = 1,     // after TCP connected
  Waiting = 2,       // after request sent
  Retrying = 3,      // after ping-pong timeout
  Disconnected = 4,  // after several ping-pong timeouts
};

export enum GameStatus {
  Waiting = 0,
  InGame = 1,
  Finished = 2,
}

export interface GameInfo {
  gameId: string;
  gameName: string;
  players: number;
  maxPlayers: number;
  nicknames: string[];
  status: GameStatus;
}

export enum PlayerStatus {
  NotReady = 0,
  Ready = 1,
  Playing = 2,
  Disconnected = 3,
}

export interface PlayerInfo {
  nickname: string;
  status: PlayerStatus;
  score: number;
  handCardId: number | null;
}

export interface PlaceInfo {
  nickname: string;
  score: number;
}
