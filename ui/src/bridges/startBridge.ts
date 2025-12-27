import type { QtSignal } from "@/types/channel";

export interface StartBridge {
  // signals
  setNickname: QtSignal<(nickname: string) => void>;

  // slots
  requestNickname: (nickname: string) => void;
}
