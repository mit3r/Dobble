import type { StateCreator } from "zustand";
import { produce } from "immer";
import { mainStore, type MainStore } from ".";
import type { GameStatus, GameInfo } from "@/types/dobble";

export type LobbySlice = {
  info: GameInfo | null;
};

export const createLobbySlice: StateCreator<MainStore, [], [], LobbySlice> = () => ({
  info: null,
});

window.bridges?.lobby.setLobbyName.connect((name: string) => {
  mainStore.setState(
    produce(({ lobby }: MainStore) => {
      if (!lobby.info) return;
      lobby.info.gameName = name;
    })
  );
});

window.bridges?.lobby.setLobbyStatus.connect((status: GameStatus) => {
  mainStore.setState(
    produce(({ lobby }: MainStore) => {
      if (!lobby.info) return;
      lobby.info.status = status;
    })
  );
});

window.bridges?.lobby.setPlayerList.connect((playerList: string[]) => {
  mainStore.setState(
    produce(({ lobby }: MainStore) => {
      if (!lobby.info) return;
      lobby.info.players = playerList.length;
      lobby.info.nicknames = playerList;
    })
  );
});
