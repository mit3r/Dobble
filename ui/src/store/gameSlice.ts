import { type GameInfo } from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type GameSliceState = {
  gameInfo: GameInfo | null;

  topPick: number | undefined;
  handPick: number | undefined;
};

export type GameSlice = GameSliceState & {
  pickTopCard: (cardId: number) => void;
  pickHandCard: (cardId: number) => void;
  clearPicks: () => void;
};

export const initialGameState: GameSliceState = {
  gameInfo: null,

  topPick: undefined,
  handPick: undefined,
};

export const createGameSlice: StateCreator<MainStore, [], [], GameSlice> = (set) => ({
  gameInfo: null,

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
