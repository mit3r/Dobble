import { useEffect } from "react";
import { mainStore } from "../../store";
import type { LobbyInfo } from "../../store/lobbySlice";
import type { Page } from "../../store/pagesSlice";

/**
 * QWebChannel type declarations - loaded globally from qwebchannel.js
 */
interface QWebChannelTransport {
  send(data: string): void;
  onmessage: ((message: { data: any }) => void) | null;
}

interface QWebChannelInstance<T extends Record<string, any> = any> {
  objects: T;
}

interface QWebChannelConstructor {
  new <T extends Record<string, any> = any>(
    transport: QWebChannelTransport,
    callback: (channel: QWebChannelInstance<T>) => void
  ): void;
}

declare global {
  interface Window {
    QWebChannel: QWebChannelConstructor;
    webBridge?: QtWebBridge;
    qt?: {
      webChannelTransport: QWebChannelTransport;
    };
  }
}

/**
 * Player object structure from Qt C++
 */
interface QtPlayer {
  name: string;
}

/**
 * Qt Signal interface - represents signal.connect() method
 */
interface QtSignal<T extends (...args: any[]) => void> {
  connect(handler: T): void;
  disconnect(handler: T): void;
}

/**
 * Interface for QWebChannel instance
 */
interface QtChannelObjects {
  webBridge: QtWebBridge;
  [key: string]: unknown;
}

/**
 * Interface representing the Qt Backend QObject exposed via QWebChannel
 * This interface should match the signals defined in backend.hpp
 *
 * @example C++ Header (backend.hpp):
 * ```cpp
 * class Backend : public QObject {
 *     Q_OBJECT
 * public:
 *     explicit Backend(QObject *parent = nullptr);
 *
 * signals:
 *     void pageChanged(const QString &page);
 *     void lobbyInfoChanged(const QString &lobbyName, const QVariantList &players, const QString &state);
 *     void lobbyCleared();
 *     void playersChanged(const QVariantList &players);
 *     void lobbyStateChanged(const QString &state);
 * };
 * ```
 */
interface QtWebBridge {
  /**
   * Signal emitted when page should change
   * C++: void pageChanged(const QString &page);
   *
   * @param page - One of: "start" | "room" | "game" | "end"
   */
  pageChanged: QtSignal<(page: Page) => void>;

  /**
   * Signal emitted when complete lobby information changes
   * C++: void lobbyInfoChanged(const QString &lobbyName, const QVariantList &players, const QString &state);
   *
   * @param lobbyName - Name of the lobby
   * @param players - Array of player objects with {name: string}
   * @param state - Lobby state: "waiting" | "playing" | "finished"
   */
  lobbyInfoChanged: QtSignal<(lobbyName: string, players: QtPlayer[], state: string) => void>;

  /**
   * Signal emitted when lobby should be cleared
   * C++: void lobbyCleared();
   */
  lobbyCleared: QtSignal<() => void>;

  /**
   * [Optional] Signal emitted when only players list changes
   * C++: void playersChanged(const QVariantList &players);
   *
   * @param players - Array of player objects with {name: string}
   */
  playersChanged?: QtSignal<(players: QtPlayer[]) => void>;

  /**
   * [Optional] Signal emitted when only lobby state changes
   * C++: void lobbyStateChanged(const QString &state);
   *
   * @param state - Lobby state: "waiting" | "playing" | "finished"
   */
  lobbyStateChanged?: QtSignal<(state: string) => void>;

  // Add public slots here if you need to call methods from JS
  // Example:
  // someMethod(): void;
}

let isQWebChannelInitialized = false;

export default function useQWebChannel() {
  useEffect(() => {
    if (isQWebChannelInitialized) {
      return;
    }

    // Sprawdź czy aplikacja działa w środowisku Qt WebEngine
    if (!window.qt || !window.qt.webChannelTransport) {
      console.warn(
        "[QWebChannel] Not running in Qt WebEngine environment. QWebChannel will not be initialized."
      );
      return;
    }

    // Sprawdź czy QWebChannel jest dostępny globalnie
    if (!window.QWebChannel) {
      console.error(
        "[QWebChannel] QWebChannel is not available. Make sure qwebchannel.js is loaded."
      );
      return;
    }

    isQWebChannelInitialized = true;
    console.log("[QWebChannel] Initializing QWebChannel...");

    try {
      new window.QWebChannel(
        window.qt.webChannelTransport,
        (channel: QWebChannelInstance<QtChannelObjects>) => {
          // Pobierz obiekt backend z C++
          const bridge = channel.objects.webBridge;

          if (!bridge) {
            console.error("[QWebChannel] Backend object not found in channel.objects");
            return;
          }

          console.log("[QWebChannel] Connected to Qt backend");

          // Podłącz się do sygnału zmiany strony
          // C++: void pageChanged(const QString &page);
          bridge.pageChanged.connect((page: Page) => {
            console.log(`[QWebChannel] Page changed to: ${page}`);
            mainStore.getState().setPage(page);
          });

          // Podłącz się do sygnału ustawienia informacji o lobby
          // C++: void lobbyInfoChanged(const QString &lobbyName, const QVariantList &players, const QString &state);
          bridge.lobbyInfoChanged.connect(
            (lobbyName: string, players: QtPlayer[], state: string) => {
              console.log("[QWebChannel] Lobby info changed:", { lobbyName, players, state });

              const lobbyInfo: LobbyInfo = {
                lobbyName,
                players: players.map((p) => ({ name: p.name })),
                state: state as LobbyInfo["state"],
              };

              mainStore.getState().setLobbyInfo(lobbyInfo);
            }
          );

          // Podłącz się do sygnału wyczyszczenia lobby
          // C++: void lobbyCleared();
          bridge.lobbyCleared.connect(() => {
            console.log("[QWebChannel] Lobby cleared");
            mainStore.getState().clearLobbyInfo();
          });

          // Opcjonalnie: sygnał do aktualizacji tylko graczy
          // C++: void playersChanged(const QVariantList &players);
          if (bridge.playersChanged) {
            bridge.playersChanged.connect((players: QtPlayer[]) => {
              console.log("[QWebChannel] Players changed:", players);
              const current = mainStore.getState().lobbyInfo;
              if (current) {
                mainStore.getState().setLobbyInfo({
                  ...current,
                  players: players.map((p) => ({ name: p.name })),
                });
              }
            });
          }

          // Opcjonalnie: sygnał do aktualizacji stanu lobby
          // C++: void lobbyStateChanged(const QString &state);
          if (bridge.lobbyStateChanged) {
            bridge.lobbyStateChanged.connect((state: string) => {
              console.log("[QWebChannel] Lobby state changed:", state);
              const current = mainStore.getState().lobbyInfo;
              if (current) {
                mainStore.getState().setLobbyInfo({
                  ...current,
                  state: state as LobbyInfo["state"],
                });
              }
            });
          }

          // Udostępnij backend globalnie (opcjonalnie, do wywoływania metod z JS)
          window.webBridge = bridge;

          console.log("[QWebChannel] All signal handlers connected");
        }
      );
    } catch (error) {
      console.error("[QWebChannel] Error initializing QWebChannel:", error);
    }
  }, []);
}
