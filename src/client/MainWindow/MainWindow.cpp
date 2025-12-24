#include "MainWindow.hpp"

MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent) {
  webView = new QWebEngineView(this);

  // Disable right-click context menu
  webView->setContextMenuPolicy(Qt::NoContextMenu);

  // Enable developer tools
  webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
  webView->settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);
  webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

  // Setup WebChannel
  webChannel = new QWebChannel(this);
  webBridge = new WebBridge(this);
  webChannel->registerObject(QStringLiteral("webBridge"), webBridge);
  webView->page()->setWebChannel(webChannel);

  // Setup custom URL scheme handler for local files
  QString distPath = QCoreApplication::applicationDirPath() + "/web";
  LocalSchemeHandler* handler = new LocalSchemeHandler(distPath, this);
  webView->page()->profile()->installUrlSchemeHandler("app", handler);

  std::cout << "Loading app from: " << distPath.toStdString() << std::endl;
  webView->setUrl(QUrl("app://localhost/index.html"));

  setCentralWidget(webView);
  resize(1024, 768);

  // Emit pageChanged signal after 5 seconds
  QTimer::singleShot(2000, this, [this]() {
    emit webBridge->pageChanged("room");
  });
}

MainWindow::~MainWindow() {
}
