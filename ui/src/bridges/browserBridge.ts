import type { QtSignal } from "@/types/channel";
import type { GameInfo } from "@/types/dobble";

export interface BrowserBridge {
  // signals
  setPage: QtSignal<(pageNumber: number, gamesList: GameInfo[]) => void>;

  // slots
  navigateToPage(pageNumber: number): void;
  joinGame(gameId: string): void;
  observeGame(gameId: string): void;
}
