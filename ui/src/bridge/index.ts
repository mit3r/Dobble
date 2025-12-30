import type { QWebChannelConstructor, QWebChannelTransport } from "@/bridge/channel";
import type { BrowserBridge } from "@/store/browserSlice";
import type { GameBridge } from "@/store/gameSlice";
import type { MainBridge } from "@/store/mainSlice";

import "@/bridge/initialize";

export interface ChannelObjects {
  main: MainBridge;
  browser: BrowserBridge;
  game: GameBridge;
}

declare global {
  interface Window {
    // QWebChannel related objects
    QWebChannel: QWebChannelConstructor;
    qt?: {
      webChannelTransport: QWebChannelTransport;
    };

    // Exposed bridges via window object
    bridges: ChannelObjects | undefined;
  }
}
