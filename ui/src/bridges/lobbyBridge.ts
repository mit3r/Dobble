import type { QtSignal } from "@/types/channel";
import type { GameStatus } from "@/types/dobble";

export interface LobbyBridge {
  // Signals
  setLobbyName: QtSignal<(lobbyName: string) => void>;
  setPlayerList: QtSignal<(playerList: string[]) => void>;
  setLobbyStatus: QtSignal<(status: GameStatus) => void>;

  // Slots
  setReadyState(isReady: boolean): void;
  quitLobby(): void;
}
