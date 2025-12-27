import type { QtSignal } from "@/types/channel";
import type { PlayerStatus } from "@/types/dobble";

export interface GameBridge {
  // signals
  setPlayerStatus: QtSignal<(nickname: string, status: PlayerStatus) => void>;
  setPlayerScore: QtSignal<(nickname: string, score: number) => void>;

  setTopCard: QtSignal<(cardId: number) => void>;
  setPlayerCard: QtSignal<(nickname: string, cardId: number) => void>;

  setLastsCards: QtSignal<(count: number) => void>;

  // slots
  match: (pick1: number, pick2: number) => void;
  quitGame: () => void;
}
