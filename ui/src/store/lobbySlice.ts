import { qwebchannelInitializer } from "@/bridge/initialize";
import type { QtSignal } from "@/bridge/channel";
import type { GameInfo, GameStatus } from "@/types/dobble";
import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";
import { produce } from "immer";

export interface LobbyBridge {
  // Signals
  onLobbyNameChanged: QtSignal<(lobbyName: string) => void>;
  onPlayersListChanged: QtSignal<(playerList: string[]) => void>;
  onLobbyStatusChanged: QtSignal<(status: GameStatus) => void>;
  onReadyStateChanged: QtSignal<(nickname: string, isReady: boolean) => void>;
  onLobbyQuit: QtSignal<() => void>;

  // Slots
  callSetReadyState(isReady: boolean): void;
  callQuitLobby(): void;
}

export type LobbySlice = {
  info: GameInfo | null;
};

export const createLobbySlice: StateCreator<MainStore, [], [], LobbySlice> = () => ({
  info: null,
});

qwebchannelInitializer.onReady(() => {
  window.bridges!.lobby.onLobbyNameChanged.connect((name: string) => {
    mainStore.setState(
      produce(({ lobby }: MainStore) => {
        if (!lobby.info) return;
        lobby.info.gameName = name;
      })
    );
  });

  window.bridges!.lobby.onLobbyStatusChanged.connect((status: GameStatus) => {
    mainStore.setState(
      produce(({ lobby }: MainStore) => {
        if (!lobby.info) return;
        lobby.info.status = status;
      })
    );
  });

  window.bridges!.lobby.onPlayersListChanged.connect((playerList: string[]) => {
    mainStore.setState(
      produce(({ lobby }: MainStore) => {
        if (!lobby.info) return;
        lobby.info.players = playerList.length;
        lobby.info.nicknames = playerList;
      })
    );
  });
});
