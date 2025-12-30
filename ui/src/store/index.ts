import { createStore } from "zustand";
import { createMainSlice, type MainSlice } from "./mainSlice";
import { createBrowserSlice, type BrowserSlice } from "./browserSlice";
import { createGameSlice, type GameSlice } from "./gameSlice";

export type MainStore = {
  main: MainSlice;
  browser: BrowserSlice;
  game: GameSlice;
};

export const mainStore = createStore<MainStore>()((...args) => ({
  main: createMainSlice(...args),
  browser: createBrowserSlice(...args),
  game: createGameSlice(...args),
}));
