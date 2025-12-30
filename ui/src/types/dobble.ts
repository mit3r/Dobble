export enum View {
  Login = 0,
  Browser = 1,
  Room = 2,
  Game = 3,
  End = 4,
}

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
