import type { StateCreator } from "zustand";
import { type MainStore, mainStore } from ".";

export type StartSlice = {
  nickname: string | null;
  loginError: string | null;
};

export const createStartSlice: StateCreator<MainStore, [], [], StartSlice> = () => ({
  nickname: null,
  loginError: null,
});

// Listen to nickname verification success from C++
window.bridges?.start.onLoginSucceeded.connect((nickname: string) => {
  console.log("Nickname verified signal received in UI:", nickname);
  mainStore.setState((state) => ({
    start: { ...state.start, nickname: nickname, loginError: null },
  }));
});

window.bridges?.start.onLoginFailed.connect((error: string) => {
  console.log("Nickname verification failed signal received in UI:", error);
  mainStore.setState((state) => ({
    start: { ...state.start, nickname: null, loginError: error },
  }));
});