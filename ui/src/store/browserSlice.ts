import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import {
  CommunicationStatus,
  ConnectionStatus,
  type ConnectionError,
  type ShortGameInfo,
} from "@/types/dobble";
import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";

export interface BrowserBridge {
  onServerConnectionStateChanged: QtSignal<(status: ConnectionStatus) => void>;
  onServerConnectionErrorOccurred: QtSignal<(error: ConnectionError) => void>;

  onServerCommunicationStateChanged: QtSignal<(status: CommunicationStatus) => void>;

  onLoginSucceeded: QtSignal<(nickname: string) => void>;
  onLoginFailed: QtSignal<(error: string) => void>;

  onPageChanged: QtSignal<(pageNumber: number, gamesList: ShortGameInfo[]) => void>;

  callVerifyNickname(nickname: string): void;
  callNavigateToPage(pageNumber: number): void;
  callJoinGame(gameId: string): void;
  callObserveGame(gameId: string): void;
}

export type BrowserSlice = {
  nicknameError: string | null;

  lobbyConnection: ConnectionStatus;
  lobbyError: ConnectionError | null;

  lobbyCommunication: CommunicationStatus;

  games: ShortGameInfo[] | null;
  pageNumber: number | null;
};

export const createBrowserSlice: StateCreator<MainStore, [], [], BrowserSlice> = () => ({
  nicknameError: null,

  lobbyConnection: ConnectionStatus.Connecting,
  lobbyError: null,

  lobbyCommunication: CommunicationStatus.None,

  games: null,
  pageNumber: null,
});

qwebchannelInitializer.onReady(() => {
  window.bridges!.browser.onServerConnectionStateChanged.connect((status: ConnectionStatus) => {
    mainStore.setState((state) => ({ browser: { ...state.browser, lobbyConnection: status } }));
  });

  window.bridges!.browser.onServerConnectionErrorOccurred.connect((error: ConnectionError) => {
    mainStore.setState((state) => ({ browser: { ...state.browser, lobbyError: error } }));
  });

  window.bridges!.browser.onServerCommunicationStateChanged.connect(
    (status: CommunicationStatus) => {
      mainStore.setState((state) => ({
        browser: { ...state.browser, lobbyCommunication: status },
      }));
    }
  );

  window.bridges!.browser.onLoginSucceeded.connect((nickname: string) => {
    mainStore.setState((state) => ({
      main: { ...state.main, nickname },
      browser: { ...state.browser, nicknameError: null },
    }));
  });

  window.bridges!.browser.onLoginFailed.connect((error: string) => {
    mainStore.setState((state) => ({
      main: { ...state.main, nickname: null },
      browser: { ...state.browser, nicknameError: error },
    }));
  });

  window.bridges!.browser.onPageChanged.connect(
    (pageNumber: number, gamesList: ShortGameInfo[]) => {
      mainStore.setState((state) => ({
        browser: { ...state.browser, pageNumber, games: gamesList },
      }));
    }
  );
});
