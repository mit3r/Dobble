#include <QCoreApplication>
#include <QDir>
#include <QFile>
#include <QMimeDatabase>
#include <QWebChannel>
#include <QWebEngineProfile>
#include <QWebEngineSettings>
#include <QWebEngineUrlRequestJob>
#include <QWebEngineUrlSchemeHandler>
#include <QWebEngineView>
#include <iostream>

class LocalSchemeHandler : public QWebEngineUrlSchemeHandler {
  public:
  LocalSchemeHandler(const QString& basePath, QObject* parent = nullptr)
      : QWebEngineUrlSchemeHandler(parent), basePath(basePath) {}

  void requestStarted(QWebEngineUrlRequestJob* request) override {
    QString path = request->requestUrl().path();
    if (path == "/") path = "/index.html";

    QString filePath = basePath + path;
    QFile* file = new QFile(filePath, request);

    if (file->open(QIODevice::ReadOnly)) {
      QMimeDatabase mimeDb;
      QString mimeType = mimeDb.mimeTypeForFile(filePath).name();
      request->reply(mimeType.toUtf8(), file);
      std::cout << "Serving: " << filePath.toStdString() << " (" << mimeType.toStdString() << ")" << std::endl;
    } else {
      std::cout << "File not found: " << filePath.toStdString() << std::endl;
      request->fail(QWebEngineUrlRequestJob::UrlNotFound);
      delete file;
    }
  }

  private:
  QString basePath;
};