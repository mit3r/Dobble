import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";
import { produce } from "immer";
import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";

export type View = "start" | "browser" | "room" | "game" | "end";

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
  view: "start",
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
