import type { QtSignal } from "@/types/channel";

export interface EndBridge {
  // signal
  setScoreboard: QtSignal<(names: string[], scores: number[]) => void>;

  // slots
  returnToBrowse(): void;
}
