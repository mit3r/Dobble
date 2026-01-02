import type { GameInfo } from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type GameSlice = {
  gameInfo: GameInfo | null;
};

export const createGameSlice: StateCreator<MainStore, [], [], GameSlice> = () => ({
  gameInfo: null,
});
