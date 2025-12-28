import type { QtSignal } from "@/types/channel";

export interface StartBridge {
  // signals
  onNicknameVerified: QtSignal<() => void>;

  // slots
  callVerifyNickname: (nickname: string) => void;
}
