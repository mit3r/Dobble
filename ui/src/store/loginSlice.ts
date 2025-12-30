import type { QtSignal } from "@/bridge/channel";
import type { StateCreator } from "zustand";
import { type MainStore, mainStore } from ".";
import { qwebchannelInitializer } from "@/bridge/initialize";

export interface LoginBridge {
  // signals
  onLoginSucceeded: QtSignal<(nickname: string) => void>;
  onLoginFailed: QtSignal<(error: string) => void>;

  // slots
  callVerifyNickname: (nickname: string) => void;
}

export type LoginSlice = {
  nickname: string | null;
  error: string | null;

  inVerification: boolean;
  verifyNickname: (nickname: string) => void;
};

export const createLoginSlice: StateCreator<MainStore, [], [], LoginSlice> = () => ({
  inVerification: false,
  nickname: null,
  error: null,

  verifyNickname: (nickname: string) => {
    mainStore.setState((state) => ({
      login: { ...state.login, inVerification: true, error: null },
    }));
    window.bridges!.login.callVerifyNickname(nickname);
  },
});

// Listen to nickname verification success from C++ - wait for initialization
qwebchannelInitializer.onReady(() => {
  window.bridges!.login.onLoginSucceeded.connect((nickname: string) => {
    console.log("Nickname verified signal received in UI:", nickname);
    mainStore.setState((state) => ({
      login: { ...state.login, nickname: nickname, error: null, inVerification: false },
    }));
  });

  window.bridges!.login.onLoginFailed.connect((error: string) => {
    console.log("Nickname verification failed signal received in UI:", error);
    mainStore.setState((state) => ({
      login: { ...state.login, nickname: null, error: error, inVerification: false },
    }));
  });
});
