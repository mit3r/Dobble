import { View } from "@/types/dobble";
import { produce } from "immer";
import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export interface MainSlice {
  nickname: string | null;
  nicknameError: string | null;

  view: View;
  setView: (view: View) => void;

  globalErrorMessage: string | null;

  alert: string | null;
  setAlert: (message: string) => void;
  clearAlert: () => void;
}

export const createMainSlice: StateCreator<MainStore, [], [], MainSlice> = (set) => ({
  nickname: null,
  nicknameError: null,
  globalErrorMessage: null,

  view: View.Login,
  setView: (view: View) =>
    set(
      produce((state: MainStore) => {
        state.main.view = view;
      })
    ),

  alert: null,
  setAlert: (message: string) =>
    set(
      produce((state: MainStore) => {
        state.main.alert = message;
      })
    ),
  clearAlert: () =>
    set(
      produce((state: MainStore) => {
        state.main.alert = null;
      })
    ),
});
