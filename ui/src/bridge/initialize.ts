import type { ChannelObjects } from ".";
import type { QWebChannelInstance } from "./channel";

type InitCallback<T extends keyof ChannelObjects> = (bridge: ChannelObjects[T]) => void;

class QWebChannelInitializer {
  private initialized = false;
  private initializationPromise: Promise<void> | null = null;
  private callbacks: {
    [K in keyof ChannelObjects]: InitCallback<K>[];
  } = {
    browser: [],
    main: [],
    game: [],
  };

  async initialize(): Promise<void> {
    if (this.initialized) return Promise.resolve();
    if (this.initializationPromise) return this.initializationPromise;

    this.initializationPromise = new Promise((resolve, reject) => {
      // Sprawdź czy aplikacja działa w środowisku Qt WebEngine
      if (!window.qt || !window.qt.webChannelTransport) {
        console.warn(
          "[QWebChannel] Not running in Qt WebEngine environment. QWebChannel will not be initialized."
        );
        reject(new Error("Not in Qt WebEngine environment"));
        return;
      }

      // Sprawdź czy QWebChannel jest dostępny globalnie
      if (!window.QWebChannel) {
        console.error(
          "[QWebChannel] QWebChannel is not available. Make sure qwebchannel.js is loaded."
        );
        reject(new Error("QWebChannel not available"));
        return;
      }

      console.log("[QWebChannel] Initializing QWebChannel...");

      try {
        new window.QWebChannel(
          window.qt.webChannelTransport,
          (channel: QWebChannelInstance<ChannelObjects>) => {
            // Pobierz obiekt backend z C++
            const bridges = channel.objects as ChannelObjects;

            if (!bridges) {
              console.error("[QWebChannel] Backend object not found in channel.objects");
              reject(new Error("Backend object not found"));
              return;
            }

            console.log("[QWebChannel] Connected to Qt backend");

            // Udostępnij backend globalnie
            window.bridges = bridges;
            this.initialized = true;

            console.log("[QWebChannel] All signal handlers connected");

            // Call all pending callbacks
            for (const [bridgeName, callbacks] of Object.entries(this.callbacks)) {
              for (const cb of callbacks) {
                try {
                  cb(window.bridges![bridgeName as keyof ChannelObjects] as any);
                } catch (error) {
                  console.error(
                    `[QWebChannel] Error in onReady callback for bridge "${bridgeName}": ${error}`
                  );
                }
              }

              this.callbacks[bridgeName as keyof ChannelObjects] = [];
            }

            resolve();
          }
        );
      } catch (error) {
        console.error("[QWebChannel] Error initializing QWebChannel:", error);
        reject(error);
      }
    });

    return this.initializationPromise;
  }

  onReady<T extends keyof ChannelObjects>(bridge: T, callback: InitCallback<T>): void {
    try {
      if (this.initialized) {
        callback(window.bridges![bridge]);
      } else {
        this.callbacks[bridge].push(callback);
      }
    } catch (error) {
      console.error("[QWebChannel] Error in onReady callback:", error);
    }
  }

  isInitialized(): boolean {
    return this.initialized;
  }
}

export const qwebchannelInitializer = new QWebChannelInitializer();

// Start initialization immediately
qwebchannelInitializer
  .initialize()
  .then(() => {
    console.log("[QWebChannel] Initialization completed");
    window.bridges?.main.callUIReady();
  })
  .catch((err) => {
    console.error("[QWebChannel] Failed to initialize:", err);
  });
