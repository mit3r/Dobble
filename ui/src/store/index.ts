import { createStore } from "zustand";
import { createLobbySlice, type LobbySlice } from "./lobbySlice";
import { createViewsSlice, type MainSlice } from "./mainSlice";
import { createLoginSlice, type LoginSlice } from "./loginSlice";
import { createBrowserSlice, type BrowserSlice } from "./browserSlice";
import { createGameSlice, type GameSlice } from "./gameSlice";
import { createEndSlice, type EndSlice } from "./endSlice";

export type MainStore = {
  views: MainSlice;
  login: LoginSlice;
  browser: BrowserSlice;
  lobby: LobbySlice;
  game: GameSlice;
  end: EndSlice;
};

export const mainStore = createStore<MainStore>()((...args) => ({
  views: createViewsSlice(...args),
  login: createLoginSlice(...args),
  browser: createBrowserSlice(...args),
  lobby: createLobbySlice(...args),
  game: createGameSlice(...args),
  end: createEndSlice(...args),
}));
