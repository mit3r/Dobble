import type { StateCreator } from "zustand";
import { type MainStore } from ".";
import type { PlaceInfo } from "@/types/dobble";

export type EndSlice = {
  scoreboard: PlaceInfo[];
};

export const createEndSlice: StateCreator<MainStore, [], [], EndSlice> = () => ({
  scoreboard: [],
});
