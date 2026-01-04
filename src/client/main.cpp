#include <QApplication>
#include <QNetworkProxy>
#include <QNetworkProxyFactory>
#include <QWebEngineSettings>
#include <QWebEngineUrlScheme>
#include <QWebEngineView>

#include "MainWindow/MainWindow.hpp"

int main(int argc, char* argv[]) {
  // Register custom URL scheme before creating QApplication
  QWebEngineUrlScheme scheme("app");
  scheme.setFlags(QWebEngineUrlScheme::SecureScheme |
                  QWebEngineUrlScheme::LocalScheme |
                  QWebEngineUrlScheme::LocalAccessAllowed |
                  QWebEngineUrlScheme::CorsEnabled);
  QWebEngineUrlScheme::registerScheme(scheme);

  // Disable GPU acceleration to avoid WSL/Linux rendering issues
  // qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --disable-software-rasterizer --no-sandbox
  // --disable-dev-shm-usage");
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox --disable-dev-shm-usage");
  // qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--no-sandbox");

  // Enable debug logging for QWebEngine
  qputenv("QTWEBENGINE_REMOTE_DEBUGGING", "9222");

  QNetworkProxyFactory::setUseSystemConfiguration(false);
  QNetworkProxy::setApplicationProxy(QNetworkProxy::NoProxy);

  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}
