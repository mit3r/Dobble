import type { QWebChannelConstructor, QWebChannelTransport } from "@/bridge/channel";
import { type BrowserBridge } from "@/store/browserBridge";
import { type GameBridge } from "@/store/gameBridge";
import { type MainBridge } from "@/store/mainBridge";

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
