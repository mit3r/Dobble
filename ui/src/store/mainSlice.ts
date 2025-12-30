import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";
import { produce } from "immer";
import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import { View } from "@/types/dobble";

export interface MainBridge {
  // signals to listen to
  onNavigated: QtSignal<(view: View) => void>;
  onError: QtSignal<(message: string) => void>;

  // slots to be called
  callCloseApp: () => void;
}

export interface MainSlice {
  view: View;
  setView: (view: View) => void;
}

export const createViewsSlice: StateCreator<MainStore, [], [], MainSlice> = (set) => ({
  view: View.Login,
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
