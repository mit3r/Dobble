import { GameStatus, type GameInfo } from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type GameSlice = {
  gameInfo: GameInfo | null;

  topPick: number | undefined;
  handPick: number | undefined;

  pickTopCard: (cardId: number) => void;
  pickHandCard: (cardId: number) => void;
  clearPicks: () => void;
};

export const createGameSlice: StateCreator<MainStore, [], [], GameSlice> = (set) => ({
  // gameInfo: null,
  gameInfo: {
    gameId: "abc123",
    gameName: "Fun Game",
    lastsCards: 5,
    status: GameStatus.InGame,
    topCardId: 10,
    players: new Array(4).fill(null).map((_, i) => ({
      nickname: `Player${i + 1}`,
      cardId: i + 5,
      matches: i * 2,
      mistakes: i,
      points: i * 10,
    })),
  },

  topPick: undefined,
  handPick: undefined,

  pickTopCard: (cardId: number) => {
    set((state) => ({ ...state, game: { ...state.game, topPick: cardId } }));
  },
  pickHandCard: (cardId: number) => {
    set((state) => ({ ...state, game: { ...state.game, handPick: cardId } }));
  },
  clearPicks: () => {
    set((state) => ({
      ...state,
      game: { ...state.game, topPick: undefined, handPick: undefined },
    }));
  },
});
