#ifndef UPLOADER_H
#define UPLOADER_H

#include <QObject>
#include "qupfile.h"
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QUrl>

class UpLoader : public QObject
{
    Q_OBJECT

public:
    UpLoader(QObject *parent = 0);
    ~UpLoader();
    bool aborted();

private:
    QUpFile *upf;
    QNetworkAccessManager *manager;
    QNetworkReply *reply;
    QString siteurl;
    bool isAborted;
    bool isInProgress;

signals:
    void started();
    void progress(qint64 bytesSent, qint64 bytesTotal);
    void finished(bool error, bool aborted, const QString &text);

public slots:
    void upload(const QString &filename, const QString &passw, const QString &descr);
    void abort();

private slots:
    void replyFinished();
};

#endif // UPLOADER_H
