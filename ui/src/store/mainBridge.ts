import type { QtSignal } from "@/bridge/channel";
import { qwebchannelInitializer } from "@/bridge/initialize";
import type { View } from "@/types/dobble";
import { mainStore } from ".";

export interface MainBridge {
  onNavigated: QtSignal<(view: View) => void>;
  onGlobalErrorOccured: QtSignal<(message: string) => void>;

  onAlert: QtSignal<(message: string) => void>;

  callMsg(message: string): void;
  callUIReady(): void;
}

qwebchannelInitializer.onReady("main", (bridge) => {
  bridge.onNavigated.connect((view: View) => {
    mainStore.getState().main.setView(view);
  });

  bridge.onGlobalErrorOccured.connect((message: string) => {
    mainStore.getState().main.globalErrorMessage = message;
  });

  bridge.onAlert.connect((message: string) => {
    mainStore.getState().main.setAlert(message);
  });
});
