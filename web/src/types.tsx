/** getstatus */
export type ClientStatus = {
  name: string;
  status: "idle" | "ready";
};

/** getinfolobby */
export type LobbyInfo = {
  page: number;
  actualGames: ActualGame[];
  prevPage: number | null;
  nextPage: number | null;
};

export type ActualGame = {
  gameId: string;
  gameName: string;
  players: number;
  maxPlayers: number;
  nicknames: string[];
  status: "waiting" | "in-game" | "finished";
};

/** getinfogamme */
export type GameInfo = {
  gameId: string;
  gameName: string;
  players: number;
  nicknames: string[];
  status: "waiting" | "in-game" | "finished";
};
