import {
  CommunicationStatus,
  ConnectionStatus,
  type ConnectionError,
  type ShortGameInfo,
} from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type BrowserSliceState = {
  lobbyIpAddress: string;
  lobbyPort: number;

  lobbyConnection: ConnectionStatus;
  lobbyError: ConnectionError | null;

  lobbyCommunication: CommunicationStatus;

  games: ShortGameInfo[] | null;
  nextPageNumber: number | null;
  currentPageNumber: number | null;
};

export type BrowserSlice = BrowserSliceState & {
  isBlocked(): boolean;

  setLobbyAddress(ip: string, port: number): void;
  connectToLobbyServer(): void;
};

export const initialBrowserState: BrowserSliceState = {
  lobbyIpAddress: "localhost",
  lobbyPort: 1500,

  lobbyConnection: ConnectionStatus.Connecting,
  lobbyError: null,

  lobbyCommunication: CommunicationStatus.None,

  games: null,
  nextPageNumber: null,
  currentPageNumber: null,
};

export const createBrowserSlice: StateCreator<MainStore, [], [], BrowserSlice> = (_, get) => ({
  ...initialBrowserState,

  isBlocked() {
    return (
      this.lobbyCommunication !== CommunicationStatus.Good ||
      this.lobbyConnection !== ConnectionStatus.Connected
    );
  },

  setLobbyAddress(ip: string, port: number) {
    this.lobbyIpAddress = ip;
    this.lobbyPort = port;
  },

  connectToLobbyServer() {
    const { lobbyIpAddress, lobbyPort } = get().browser;
    window.bridges?.browser.callConnectToLobby(lobbyIpAddress, lobbyPort);
  },
});