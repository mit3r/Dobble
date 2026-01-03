export enum View {
  Login,
  Browser,
  Room,
  Game,
  End,
}

export enum ConnectionStatus {
  Connecting,
  HostLookup,
  Connected,
  Retrying,
  Disconnected,
}

export enum ConnectionError {
  None,
  ConnectionRefused,
  HostNotFound,
  NetworkError,
  Timeout,
  UnknownError,
}

export enum CommunicationStatus {
  None, // before TCP connected
  Good, // after TCP connected
  Waiting, // after request sent
  Retrying, // after ping-pong timeout
  Failed, // after several ping-pong timeouts
}

export enum Role {
  Play,
  Spectate,
}

export enum GameStatus {
  Waiting,
  InGame,
  Finished,
}

export interface ShortGameInfo {
  gameId: string;
  gameName: string;
  players: number;
  maxPlayers: number;
  status: GameStatus;
}

export interface PlayerGameInfo {
  nickname: string;
  cardId: number;
  points: number; // matches - mistakes
  matches: number; // total correct matches
  mistakes: number; // total mistakes
}

export interface GameInfo {
  gameId: string;
  gameName: string;

  players: PlayerGameInfo[];

  status: GameStatus;

  topCardId: number;
  lastsCards: number;
}
