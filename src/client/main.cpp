#include <QApplication>
#include <QWebEngineUrlScheme>

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
  qputenv("QTWEBENGINE_CHROMIUM_FLAGS", "--disable-gpu --no-sandbox --disable-software-rasterizer --disable-dev-shm-usage");

  QApplication app(argc, argv);

  MainWindow window;
  window.show();

  return app.exec();
}
