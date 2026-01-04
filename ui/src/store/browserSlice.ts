import {
  CommunicationStatus,
  ConnectionStatus,
  View,
  type ConnectionError,
  type ShortGameInfo,
} from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type BrowserSliceState = {
  lobbyAddress: {
    ip: string;
    port: number;
  } | null;

  lobbyConnection: ConnectionStatus;
  lobbyError: ConnectionError | null;

  lobbyCommunication: CommunicationStatus;

  games: ShortGameInfo[] | null;
  nextPageNumber: number | null;
  currentPageNumber: number | null;
};

export type BrowserSlice = BrowserSliceState & {
  isBlocked(): boolean;

  clearNickname(): void;
  clearLobbyAddress(): void;
  setLobbyAddress(ip: string, port: number): void;
  connectToLobbyServer(): void;
};

export const initialBrowserState: BrowserSliceState = {
  lobbyAddress: null,

  lobbyConnection: ConnectionStatus.Connecting,
  lobbyError: null,

  lobbyCommunication: CommunicationStatus.None,

  games: null,
  nextPageNumber: null,
  currentPageNumber: null,
};

export const createBrowserSlice: StateCreator<MainStore, [], [], BrowserSlice> = (set, get) => ({
  ...initialBrowserState,

  isBlocked() {
    return (
      this.lobbyCommunication !== CommunicationStatus.Good ||
      this.lobbyConnection !== ConnectionStatus.Connected
    );
  },

  clearNickname() {
    set((state) => ({ main: { ...state.main, nickname: null, nicknameError: null }}))
  },

  clearLobbyAddress() {
    set((state) => ({ browser: { ...state.browser, lobbyAddress: null } }));
    get().browser.clearNickname();
    get().main.setView(View.Login);
  },

  setLobbyAddress(ip: string, port: number) {
    set((state) => ({
      browser: { ...state.browser, lobbyAddress: { ip, port } },
    }));
  },

  connectToLobbyServer() {
    const address = get().browser.lobbyAddress;
    if (address === null) return;
    window.bridges?.browser.callConnectToLobby(address.ip, address.port);
  },
});