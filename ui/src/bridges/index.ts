import type { BrowserBridge } from "./browserBridge";
import type { EndBridge } from "./endBridge";
import type { GameBridge } from "./gameBridge";
import type { LobbyBridge } from "./lobbyBridge";
import type { MainBridge } from "./mainBridge";
import type { StartBridge } from "./startBridge";
import type {
  QWebChannelConstructor,
  QWebChannelInstance,
  QWebChannelTransport,
} from "@/types/channel";

interface ChannelObjects {
  main: MainBridge;
  start: StartBridge;
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

// run-once

(function initializeQWebChannel() {
  // Sprawdź czy aplikacja działa w środowisku Qt WebEngine
  if (!window.qt || !window.qt.webChannelTransport)
    return console.warn(
      "[QWebChannel] Not running in Qt WebEngine environment. QWebChannel will not be initialized."
    );

  // Sprawdź czy QWebChannel jest dostępny globalnie
  if (!window.QWebChannel)
    return console.error(
      "[QWebChannel] QWebChannel is not available. Make sure qwebchannel.js is loaded."
    );

  console.log("[QWebChannel] Initializing QWebChannel...");

  try {
    new window.QWebChannel(
      window.qt.webChannelTransport,
      (channel: QWebChannelInstance<ChannelObjects>) => {
        // Pobierz obiekt backend z C++
        const bridges = channel.objects;

        if (!bridges)
          return console.error("[QWebChannel] Backend object not found in channel.objects");
        else console.log("[QWebChannel] Connected to Qt backend");

        // Udostępnij backend globalnie
        window.bridges = bridges;
        console.log("[QWebChannel] All signal handlers connected");
      }
    );
  } catch (error) {
    console.error("[QWebChannel] Error initializing QWebChannel:", error);
  }
})();
