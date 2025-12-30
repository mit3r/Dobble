import type { BrowserBridge } from "@/store/browserSlice";
import type { EndBridge } from "@/store/endSlice";
import type { GameBridge } from "@/store/gameSlice";
import type { LobbyBridge } from "@/store/lobbySlice";
import type { MainBridge } from "@/store/mainSlice";
import type { LoginBridge } from "@/store/loginSlice";
import type { QWebChannelConstructor, QWebChannelTransport } from "@/bridge/channel";

import "@/bridge/initialize";

export interface ChannelObjects {
  main: MainBridge;
  login: LoginBridge;
  browser: BrowserBridge;
  lobby: LobbyBridge;
  game: GameBridge;
  end: EndBridge;
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
