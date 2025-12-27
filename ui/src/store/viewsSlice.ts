import type { StateCreator } from "zustand";
import type { MainStore } from ".";
import { produce } from "immer";

export type View = "start" | "browser" | "room" | "game" | "end";

export interface ViewsSlice {
  view: View;
  setView: (view: View) => void;
}

export const createViewsSlice: StateCreator<MainStore, [], [], ViewsSlice> = (set) => ({
  view: "start",
  setView: (view: View) =>
    set(
      produce((state: MainStore) => {
        state.views.view = view;
      })
    ),
});
