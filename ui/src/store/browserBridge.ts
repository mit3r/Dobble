import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import type {
  CommunicationStatus,
  ConnectionError,
  ConnectionStatus,
  ShortGameInfo,
} from "@/types/dobble";
import { mainStore } from ".";

export interface BrowserBridge {
  onServerConnectionStateChanged: QtSignal<(status: ConnectionStatus) => void>;
  onServerConnectionErrorOccured: QtSignal<(error: ConnectionError) => void>;

  onServerCommunicationStateChanged: QtSignal<(status: CommunicationStatus) => void>;

  onLoginSucceeded: QtSignal<(nickname: string) => void>;
  onLoginFailed: QtSignal<(error: string) => void>;

  onPageChanged: QtSignal<
    (gamesList: ShortGameInfo[], currentPageNumber: number, nextPageNumber: number) => void
  >;

  callConnectToLobby(ip: string, port: number): void;
  callVerifyNickname(nickname: string): void;
  callNavigateToPage(pageNumber: number): void;
  callCreateGame(gameName: string, maxPlayers: number): void;
}

qwebchannelInitializer.onReady("browser", (bridge) => {
  bridge.onServerConnectionStateChanged.connect((status: ConnectionStatus) => {
    mainStore.setState((state) => ({
      browser: { ...state.browser, lobbyConnection: status, lobbyError: null },
    }));
  });

  bridge.onServerConnectionErrorOccured.connect((error: ConnectionError) => {
    mainStore.setState((state) => ({ browser: { ...state.browser, lobbyError: error } }));
  });

  bridge.onServerCommunicationStateChanged.connect((status: CommunicationStatus) => {
    mainStore.setState((state) => ({
      browser: { ...state.browser, lobbyCommunication: status },
    }));
  });

  bridge.onLoginSucceeded.connect((nickname: string) => {
    mainStore.setState((state) => ({
      main: { ...state.main, nickname, nicknameError: null },
    }));
  });

  bridge.onLoginFailed.connect((error: string) => {
    mainStore.setState((state) => ({
      main: { ...state.main, nickname: null, nicknameError: error },
    }));
  });

  bridge.onPageChanged.connect(
    (gamesList: ShortGameInfo[], currentPageNumber: number, nextPageNumber: number) => {
      const next = nextPageNumber > 0 ? nextPageNumber : null;
      mainStore.setState((state) => ({
        browser: {
          ...state.browser,
          nextPageNumber: next,
          games: gamesList,
          currentPageNumber: currentPageNumber,
        },
      }));
    }
  );
});