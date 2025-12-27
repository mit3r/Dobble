import type { QtSignal } from "@/types/channel";

export interface MainBridge {
  // signals to listen to
  setPage: QtSignal<(page: string) => void>;
  setError: QtSignal<(message: string) => void>;

  // slots to be called
  close: () => void;
}
