type InitCallback = () => void;

class QWebChannelInitializer {
  private initialized = false;
  private initializationPromise: Promise<void> | null = null;
  private callbacks: InitCallback[] = [];

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
        new window.QWebChannel(window.qt.webChannelTransport, (channel) => {
          // Pobierz obiekt backend z C++
          const bridges = channel.objects;

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
          this.callbacks.forEach((cb) => cb());
          this.callbacks = [];

          resolve();
        });
      } catch (error) {
        console.error("[QWebChannel] Error initializing QWebChannel:", error);
        reject(error);
      }
    });

    return this.initializationPromise;
  }

  onReady(callback: InitCallback): void {
    if (this.initialized) {
      callback();
    } else {
      this.callbacks.push(callback);
    }
  }

  isInitialized(): boolean {
    return this.initialized;
  }
}

export const qwebchannelInitializer = new QWebChannelInitializer();

// Start initialization immediately
qwebchannelInitializer.initialize().catch((err) => {
  console.error("[QWebChannel] Failed to initialize:", err);
});
