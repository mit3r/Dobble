import { createStore } from "zustand";
import { createLobbySlice, type LobbySlice } from "./lobbySlice";
import { createViewsSlice, type ViewsSlice } from "./viewsSlice";
import { createStartSlice, type StartSlice } from "./startSlice";
import { createBrowserSlice, type BrowserSlice } from "./browserSlice";
import { createGameSlice, type GameSlice } from "./gameSlice";
import { createEndSlice, type EndSlice } from "./endSlice";

export type MainStore = {
  views: ViewsSlice;
  start: StartSlice;
  browser: BrowserSlice;
  lobby: LobbySlice;
  game: GameSlice;
  end: EndSlice;
};

export const mainStore = createStore<MainStore>()((...args) => ({
  views: createViewsSlice(...args),
  start: createStartSlice(...args),
  browser: createBrowserSlice(...args),
  lobby: createLobbySlice(...args),
  game: createGameSlice(...args),
  end: createEndSlice(...args),
}));
