import { View } from "@/types/dobble";
import { produce } from "immer";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

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
