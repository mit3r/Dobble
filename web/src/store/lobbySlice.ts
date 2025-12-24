import type { StateCreator } from "zustand";
import type { MainStore } from ".";

export type LobbyPlayer = {
  name: string;
};

export type LobbyState = "waiting" | "playing" | "finished";

export type LobbyInfo = {
  lobbyName: string;
  players: LobbyPlayer[];
  state: LobbyState;
};

export type LobbySlice = {
  lobbyInfo: LobbyInfo | null;
  setLobbyInfo: (info: LobbyInfo) => void;
  clearLobbyInfo: () => void;
};

export const createLobbySlice: StateCreator<MainStore, [], [], LobbySlice> = (set) => ({
  lobbyInfo: null,
  setLobbyInfo: (info: LobbyInfo) => set({ lobbyInfo: info }),
  clearLobbyInfo: () => set({ lobbyInfo: null }),
});
