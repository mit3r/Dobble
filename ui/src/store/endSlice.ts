import type { StateCreator } from "zustand";
import { type MainStore } from ".";
import type { PlaceInfo } from "@/types/dobble";
import type { QtSignal } from "@/bridge/channel";

export interface EndBridge {
  // signal
  setScoreboard: QtSignal<(names: string[], scores: number[]) => void>;

  // slots
  returnToBrowse(): void;
}

export type EndSlice = {
  scoreboard: PlaceInfo[];
};

export const createEndSlice: StateCreator<MainStore, [], [], EndSlice> = () => ({
  scoreboard: [],
});
