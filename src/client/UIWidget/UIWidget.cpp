#include "UIWidget.hpp"

UIWidget::UIWidget(QWidget* parent) : QWidget(parent) {
  webView = new QWebEngineView(this);

  // Layout
  layout = new QVBoxLayout(this);
  layout->setContentsMargins(0, 0, 0, 0);
  layout->addWidget(webView);
  setLayout(layout);

  // Disable right-click context menu
  webView->setContextMenuPolicy(Qt::NoContextMenu);

  // Enable developer tools
  webView->settings()->setAttribute(QWebEngineSettings::JavascriptEnabled, true);
  webView->settings()->setAttribute(QWebEngineSettings::ErrorPageEnabled, true);
  webView->settings()->setAttribute(QWebEngineSettings::LocalContentCanAccessFileUrls, true);

  // Setup WebChannel
  webChannel = new QWebChannel(this);

  this->mainBridge = new MainBridge(this);
  this->browserBridge = new BrowserBridge(this);
  this->gameBridge = new GameBridge(this);

  webChannel->registerObject(QStringLiteral("main"), mainBridge);
  webChannel->registerObject(QStringLiteral("browser"), browserBridge);
  webChannel->registerObject(QStringLiteral("game"), gameBridge);

  webView->page()->setWebChannel(webChannel);

  // Setup custom URL scheme handler for local files
  QString distPath = QCoreApplication::applicationDirPath() + "/ui";
  LocalSchemeHandler* handler = new LocalSchemeHandler(distPath, this);
  webView->page()->profile()->installUrlSchemeHandler("app", handler);
  webView->setUrl(QUrl("app://localhost/index.html"));

  // std::cout << "Loading app from: " << distPath.toStdString() << std::endl;
}

UIWidget::~UIWidget() {
}
