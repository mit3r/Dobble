/**
 * QWebChannel type declarations - loaded globally from qwebchannel.js
 */
export interface QWebChannelTransport {
  send(data: string): void;
  onmessage: ((message: { data: any }) => void) | null;
}

/**
 * QWebChannel instance interface
 */
export interface QWebChannelInstance<T extends Record<string, any> = any> {
  objects: T;
}

/**
 * QWebChannel constructor interface
 */
export interface QWebChannelConstructor {
  new <T extends Record<string, any> = any>(
    transport: QWebChannelTransport,
    callback: (channel: QWebChannelInstance<T>) => void
  ): void;
}

/**
 * Qt Signal interface - represents signal.connect() method
 */
export interface QtSignal<T extends (...args: any[]) => void> {
  connect(handler: T): void;
  disconnect(handler: T): void;
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
