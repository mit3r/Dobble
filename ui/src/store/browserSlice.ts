import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";
import type { GameInfo } from "@/types/dobble";
import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";

export interface BrowserBridge {
  // signals
  onPageChanged: QtSignal<(pageNumber: number, gamesList: GameInfo[]) => void>;
  onErrorOccurred: QtSignal<(message: string) => void>;

  // slots
  callNavigateToPage(pageNumber: number): void;
  callJoinGame(gameId: string): void;
  callObserveGame(gameId: string): void;
}

export type BrowserSlice = {
  games: GameInfo[] | null;
  pageNumber: number | null;
};

export const createBrowserSlice: StateCreator<MainStore, [], [], BrowserSlice> = () => ({
  games: null,
  pageNumber: null,
});

qwebchannelInitializer.onReady(() => {
  window.bridges!.browser.onPageChanged.connect((pageNumber, games) => {
    mainStore.setState({ browser: { pageNumber, games } });
  });
});
