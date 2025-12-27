import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";
import type { GameInfo } from "@/types/dobble";

export type BrowserSlice = {
  games: GameInfo[] | null;
  pageNumber: number | null;
};

export const createBrowserSlice: StateCreator<MainStore, [], [], BrowserSlice> = () => ({
  games: null,
  pageNumber: null,
});

window.bridges?.browser.setPage.connect((pageNumber, games) => {
  mainStore.setState({ browser: { pageNumber, games } });
});
