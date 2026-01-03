import { ConnectionError, ConnectionStatus, type GameInfo } from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type GameSliceState = {
  gameServerConnectionStatus: ConnectionStatus;
  gameServerConnectionError: ConnectionError | null;
  gameServerCommunicationStatus: ConnectionStatus;

  gameInfo: GameInfo | null;

  topPick: number | undefined;
  handPick: number | undefined;
};

export type GameSlice = GameSliceState & {
  pickTopCard: (cardId: number) => void;
  pickHandCard: (cardId: number) => void;
  clearPicks: () => void;
  matchCards: () => void;
};

export const initialGameState: GameSliceState = {
  gameServerConnectionStatus: ConnectionStatus.Disconnected,
  gameServerConnectionError: null,
  gameServerCommunicationStatus: ConnectionStatus.Disconnected,

  gameInfo: null,

  topPick: undefined,
  handPick: undefined,
};

export const createGameSlice: StateCreator<MainStore, [], [], GameSlice> = (set, get) => ({
  ...initialGameState,

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
  matchCards: () => {
    const handPick = get().game.handPick;
    const topPick = get().game.topPick;
    const turnId = get().game.gameInfo?.turnId;

    if (handPick === undefined || topPick === undefined) return;
    if (handPick !== topPick) return;
    if (turnId === undefined) return;

    window.bridges?.game.callMatchCards(turnId, handPick);
  },
});
