import {
  CommunicationStatus,
  ConnectionStatus,
  type ConnectionError,
  type ShortGameInfo,
} from "@/types/dobble";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type BrowserSlice = {
  nicknameError: string | null;

  lobbyConnection: ConnectionStatus;
  lobbyError: ConnectionError | null;

  lobbyCommunication: CommunicationStatus;

  games: ShortGameInfo[] | null;
  nextPageNumber: number | null;
  currentPageNumber: number | null;
};

export const createBrowserSlice: StateCreator<MainStore, [], [], BrowserSlice> = () => ({
  nicknameError: null,

  lobbyConnection: ConnectionStatus.Connecting,
  lobbyError: null,

  lobbyCommunication: CommunicationStatus.None,

  games: null,
  nextPageNumber: null,
  currentPageNumber: null,
});
