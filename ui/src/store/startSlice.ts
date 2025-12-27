import type { StateCreator } from "zustand";
import { mainStore, type MainStore } from ".";

export type StartSlice = {
  nickname: string | null;
};

export const createStartSlice: StateCreator<MainStore, [], [], StartSlice> = () => ({
  nickname: null,
});

window.bridges?.start.setNickname.connect((nickname: string) => {
  mainStore.setState({ start: { nickname } });
});
