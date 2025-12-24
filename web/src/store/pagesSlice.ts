import type { StateCreator } from "zustand";
import type { MainStore } from ".";

export type Page = "start" | "room" | "game" | "end";

export interface PageSlice {
  page: Page;
  setPage: (page: Page) => void;
}

export const createPageSlice: StateCreator<MainStore, [], [], PageSlice> = (set) => ({
  page: "start",
  setPage: (page: Page) => set({ page }),
});
