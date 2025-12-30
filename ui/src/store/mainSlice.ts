import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";
import { produce } from "immer";
import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import { ConnectionError, ConnectionStatus, View } from "@/types/dobble";

export interface MainBridge {
  // signals to listen to
  onNavigated: QtSignal<(view: View) => void>;
  onLobbyServerStateChanged: QtSignal<(status: ConnectionStatus) => void>;
  onLobbyServerErrorOccured: QtSignal<(error: ConnectionError) => void>;
  // slots to be called
  callCloseApp: () => void;
}

export interface MainSlice {
  view: View;
  setView: (view: View) => void;

  lobbyServerStatus: ConnectionStatus;
  lobbyServerError: ConnectionError | null;
}

export const createMainSlice: StateCreator<MainStore, [], [], MainSlice> = (set) => ({
  view: View.Login,
  lobbyServerStatus: ConnectionStatus.Connecting,
  lobbyServerError: null,

  setView: (view: View) =>
    set(
      produce((state: MainStore) => {
        state.views.view = view;
      })
    ),
});

qwebchannelInitializer.onReady(() => {
  window.bridges!.main.onNavigated.connect((view: View) => {
    mainStore.getState().views.setView(view);
  });
});
