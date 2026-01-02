import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import type { GameInfo } from "@/types/dobble";
import { mainStore } from ".";


export interface GameBridge {
  onGameInfoChanged: QtSignal<(gameInfo: GameInfo) => void>;
  onMatchResult: QtSignal<(isMatch: boolean) => void>;
  onGameQuit: QtSignal<() => void>;

  // slots
  callMatchCards: (pick1: number, pick2: number) => void;
  callQuitGame: () => void;
}

qwebchannelInitializer.onReady(() => {
  window.bridges!.game.onGameInfoChanged.connect((gameInfo: GameInfo) => {
    mainStore.setState((state) => ({ game: { ...state.game, gameInfo } }));
  });

  window.bridges!.game.onMatchResult.connect((_isMatch: boolean) => {
    // no-op for now
  });

  window.bridges!.game.onGameQuit.connect(() => {
    mainStore.setState((state) => ({ game: { ...state.game, gameInfo: null } }));
  });
});