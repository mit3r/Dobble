import type { PlayerInfo } from "@/types/dobble";
import { produce } from "immer";
import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";

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

window.bridges?.game.setTopCard.connect((cardId: number) => {
  mainStore.setState(
    produce(({ game }: MainStore) => {
      game.topCardId = cardId;
    })
  );
});

window.bridges?.game.setLastsCards.connect((cardId: number) => {
  mainStore.setState(
    produce(({ game }: MainStore) => {
      game.lastCards = cardId;
    })
  );
});

window.bridges?.game.setPlayerStatus.connect((nickname, status) => {
  mainStore.setState(
    produce(({ game }: MainStore) => {
      if (!game.players) return;
      const player = game.players.find((p) => p.nickname === nickname);
      if (player) player.status = status;
    })
  );
});

window.bridges?.game.setPlayerScore.connect((nickname, score) => {
  mainStore.setState(
    produce(({ game }: MainStore) => {
      if (!game.players) return;
      const player = game.players.find((p) => p.nickname === nickname);
      if (player) player.score = score;
    })
  );
});
