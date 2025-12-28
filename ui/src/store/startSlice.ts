import type { StateCreator } from "zustand";
import { type MainStore } from ".";

export type StartSlice = {
  nickname: string | null;
};

export const createStartSlice: StateCreator<MainStore, [], [], StartSlice> = () => ({
  nickname: null,
});

window.bridges?.start.onNicknameVerified.connect(() => {
  // mainStore.setState({ start: { nickname } });
  console.log("Nickname verified signal received in UI.");
});
