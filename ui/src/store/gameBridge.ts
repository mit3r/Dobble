import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import type {
  CommunicationStatus,
  ConnectionError,
  ConnectionStatus,
  GameInfo,
  Role,
} from "@/types/dobble";
import { mainStore } from ".";

export interface GameBridge {
  onServerConnectionStateChanged: QtSignal<(status: ConnectionStatus) => void>;
  onServerConnectionErrorOccured: QtSignal<(error: ConnectionError) => void>;
  onServerCommunicationStateChanged: QtSignal<(status: CommunicationStatus) => void>;

  onGameInfoChanged: QtSignal<(gameInfo: GameInfo) => void>;
  onMatchResult: QtSignal<(isMatch: boolean) => void>;
  onGameQuit: QtSignal<() => void>;

  // slots
  callJoinGame(ip: string, port: number, gameId: string, role: Role): void;
  callStartGame: () => void;
  callMatchCards: (turnId: string, symbolId: number) => void;
  callQuitGame: () => void;
}

qwebchannelInitializer.onReady("game", (bridge) => {
  bridge.onGameInfoChanged.connect((gameInfo: GameInfo) => {
    mainStore.setState((state) => ({ game: { ...state.game, gameInfo } }));
  });

  bridge.onMatchResult.connect((_isMatch: boolean) => {
    // no-op for now
  });

  bridge.onGameQuit.connect(() => {
    mainStore.setState((state) => ({ game: { ...state.game, gameInfo: null } }));
  });
});