import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import type { View } from "@/types/dobble";
import { mainStore } from ".";

export interface MainBridge {
  onNavigated: QtSignal<(view: View) => void>;
  onGlobalErrorOccurred: QtSignal<(message: string) => void>;
}

qwebchannelInitializer.onReady(() => {
  window.bridges!.main.onNavigated.connect((view: View) => {
    mainStore.getState().main.setView(view);
  });

  window.bridges!.main.onGlobalErrorOccurred.connect((message: string) => {
    mainStore.getState().main.globalErrorMessage = message;
  });
});
