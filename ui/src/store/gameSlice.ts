import type { PlayerInfo, PlayerStatus } from "@/types/dobble";
import { produce } from "immer";
import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";
import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";

export interface GameBridge {
  // signals
  onPlayerStatusChanged: QtSignal<(nickname: string, status: PlayerStatus) => void>;
  onPlayerScoreChanged: QtSignal<(nickname: string, score: number) => void>;

  onTopCardChanged: QtSignal<(cardId: number) => void>;
  onPlayerCardChanged: QtSignal<(nickname: string, cardId: number) => void>;

  onLastsCardsChanged: QtSignal<(count: number) => void>;

  // slots
  callMatchCards: (pick1: number, pick2: number) => void;
  callQuitGame: () => void;
}

export type GameSlice = {
  topCardId: number | null;
  lastCards: number | null;
  players: PlayerInfo[] | null;
};

export const createGameSlice: StateCreator<MainStore, [], [], GameSlice> = () => ({
  topCardId: null,
  lastCards: null,
  players: null,
});

qwebchannelInitializer.onReady(() => {
  window.bridges!.game.onTopCardChanged.connect((cardId: number) => {
    mainStore.setState(
      produce(({ game }: MainStore) => {
        game.topCardId = cardId;
      })
    );
  });

  window.bridges!.game.onLastsCardsChanged.connect((cardId: number) => {
    mainStore.setState(
      produce(({ game }: MainStore) => {
        game.lastCards = cardId;
      })
    );
  });

  window.bridges!.game.onPlayerStatusChanged.connect((nickname, status) => {
    mainStore.setState(
      produce(({ game }: MainStore) => {
        if (!game.players) return;
        const player = game.players.find((p) => p.nickname === nickname);
        if (player) player.status = status;
      })
    );
  });

  window.bridges!.game.onPlayerScoreChanged.connect((nickname, score) => {
    mainStore.setState(
      produce(({ game }: MainStore) => {
        if (!game.players) return;
        const player = game.players.find((p) => p.nickname === nickname);
        if (player) player.score = score;
      })
    );
  });
});
