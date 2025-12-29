import type { QtSignal } from "@/types/channel";

export interface StartBridge {
  // signals
  onLoginSucceeded: QtSignal<(nickname: string) => void>;
  onLoginFailed: QtSignal<(error: string) => void>;

  // slots
  callVerifyNickname: (nickname: string) => void;
}
