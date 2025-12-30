import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import { View } from "@/types/dobble";
import { produce } from "immer";
import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";

export interface MainBridge {
  onNavigated: QtSignal<(view: View) => void>;
  onGlobalErrorOccurred: QtSignal<(message: string) => void>;
}

export interface MainSlice {
  nickname: string | null;

  view: View;
  setView: (view: View) => void;

  globalErrorMessage: string | null;
}

export const createMainSlice: StateCreator<MainStore, [], [], MainSlice> = (set) => ({
  nickname: null,
  globalErrorMessage: null,

  view: View.Login,
  setView: (view: View) =>
    set(
      produce((state: MainStore) => {
        state.main.view = view;
      })
    ),
});

qwebchannelInitializer.onReady(() => {
  window.bridges!.main.onNavigated.connect((view: View) => {
    mainStore.getState().main.setView(view);
  });

  window.bridges!.main.onGlobalErrorOccurred.connect((message: string) => {
    mainStore.getState().main.globalErrorMessage = message;
  });
});
