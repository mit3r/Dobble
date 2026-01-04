import { ConnectionError, ConnectionStatus, MatchResult, type GameInfo } from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type GameSliceState = {
  gameServerConnectionStatus: ConnectionStatus;
  gameServerConnectionError: ConnectionError | null;
  gameServerCommunicationStatus: ConnectionStatus;

  gameInfo: GameInfo | null;

  topPick: number | undefined;
  handPick: number | undefined;

  matchResult: MatchResult | null;
};

export type GameSlice = GameSliceState & {
  pickTopCard: (cardId: number) => void;
  pickHandCard: (cardId: number) => void;
  clearPicks: () => void;
  matchCards: () => void;
  setMatchResult: (result: MatchResult) => void;
  clearMatchResult: () => void;
};

export const initialGameState: GameSliceState = {
  gameServerConnectionStatus: ConnectionStatus.Disconnected,
  gameServerConnectionError: null,
  gameServerCommunicationStatus: ConnectionStatus.Disconnected,

  gameInfo: null,
  // gameInfo: {
  //   gameId: "cos",
  //   lastsCards: 20,
  //   topCardId: 1,
  //   turnId: "123",
  //   players: [
  //     {
  //       nickname: "Player1",
  //       cardId: 2,
  //       matches: 0,
  //       mistakes: 0,
  //       points: 0,
  //     },
  //   ],
  //   status: GameStatus.Finished,
  //   winnerNickname: "Play123",
  // },

  topPick: undefined,
  handPick: undefined,

  matchResult: null,
};

export const createGameSlice: StateCreator<MainStore, [], [], GameSlice> = (set, get) => ({
  ...initialGameState,

  pickTopCard: (cardId: number) => {
    set((state) => ({ ...state, game: { ...state.game, topPick: cardId } }));
    get().game.matchCards();
  },
  pickHandCard: (cardId: number) => {
    set((state) => ({ ...state, game: { ...state.game, handPick: cardId } }));
    get().game.matchCards();
  },
  clearPicks: () => {
    set((state) => ({
      ...state,
      game: { ...state.game, topPick: undefined, handPick: undefined },
    }));
  },
  matchCards: () => {
    const turnId = get().game.gameInfo?.turnId;
    if (turnId === undefined) return;

    const handPick = get().game.handPick;
    const topPick = get().game.topPick;
    if (handPick === undefined || topPick === undefined) return;

    // const player = get().game.gameInfo?.players.find((p) => p.nickname === get().main.nickname);

    // window.bridges?.main.callMsg(
    //   `Matching cards: topCard=${get().game.gameInfo?.topCardId}, handCard=${player?.cardId}`
    // );
    // window.bridges?.main.callMsg(`Picks: topPick=${topPick}, handPick=${handPick}`);

    if (handPick !== topPick) return window.bridges?.game.callMatch(turnId, -1); // to powinienn robić server, ale czas nagli!
    window.bridges?.game.callMatch(turnId, handPick);
  },
  setMatchResult: (result: MatchResult) => {
    set((state) => ({ ...state, game: { ...state.game, matchResult: result } }));
    get().game.clearPicks();
  },
  clearMatchResult: () => {
    set((state) => ({ ...state, game: { ...state.game, matchResult: null } }));
  },
});
