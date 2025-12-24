import { createStore } from "zustand";
import { createLobbySlice, type LobbySlice } from "./lobbySlice";
import { createPageSlice, type PageSlice } from "./pagesSlice";

export type MainStore = PageSlice & LobbySlice;

export const mainStore = createStore<MainStore>()((...args) => ({
  ...createPageSlice(...args),
  ...createLobbySlice(...args),
}));
