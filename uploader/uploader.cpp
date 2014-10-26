#include "uploader.h"

UpLoader::UpLoader(QObject *parent) : QObject(parent)
{
    manager = new QNetworkAccessManager(this);
    reply = 0;
    upf = 0;
    isAborted = false;
    isInProgress = false;
}
UpLoader::~UpLoader()
{

}
bool UpLoader::aborted()
{
    return isAborted;
}

/////////////////////////////public slots://///////////////////////////////////////////

void UpLoader::upload(const QString &filename, const QString &passw, const QString &descr)
{
    isAborted = false;
    QByteArray boundaryRegular(QString("--"+QString::number(qrand(), 10)).toAscii());
    QByteArray boundary("\r\n--"+boundaryRegular+"\r\n");
    QByteArray boundaryLast("\r\n--"+boundaryRegular+"--\r\n");

    QString returl = QString("http://node1.metadisk.org");
    QUrl url(QString(returl+"/api/upload/"));
    QNetworkRequest request(url);
    request.setRawHeader("Host", url.encodedHost());
    request.setRawHeader("User-Agent", "Mozilla/5.0 (Windows; U; Windows NT 5.1; ru; rv:1.9.1.3) Gecko/20090824 Firefox/3.5.3 (.NET CLR 3.5.30729)");
    request.setRawHeader("Accept", "text/html,application/xhtml+xml,application/xml;q=0.9,*/*;q=0.8");
    request.setRawHeader("Accept-Language", "ru,en-us;q=0.7,en;q=0.3");
    request.setRawHeader("Accept-Encoding", "gzip,deflate");
    request.setRawHeader("Accept-Charset", "windows-1251,utf-8;q=0.7,*;q=0.7");
    request.setRawHeader("Keep-Alive", "300");
    request.setRawHeader("Connection", "keep-alive");
    request.setRawHeader("Content-Type", QByteArray("multipart/form-data; boundary=").append(boundaryRegular));

    QByteArray mimedata1("--"+boundaryRegular+"\r\n");
    mimedata1.append("Content-Disposition: form-data; name=\"action\"\r\n\r\n");
    mimedata1.append("file");
    mimedata1.append(boundary);
    mimedata1.append("Content-Disposition: form-data; file=" + filename.toUtf8());
    mimedata1.append("Content-Type: application/octet-stream\r\n\r\n");

    upf = new QUpFile(filename, mimedata1, this);
    if (upf->openFile())
    {
        reply = manager->post(request, upf);
        connect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SIGNAL(progress(qint64,qint64)));
        connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
        isInProgress = true;
        emit started();
    } else
    {
        emit finished(true, false, tr("Error: can't open file %1").arg(filename));
    }
}

void UpLoader::abort()
{
    isAborted = true;
    if (reply && isInProgress)
        reply->abort();
}

/////////////////////////////private slots:////////////////////////////////////////////
void UpLoader::replyFinished()
{
    isInProgress = false;
    if (upf)
    {
        upf->close();
        delete upf;
        upf = 0;
    }
    disconnect(reply, SIGNAL(uploadProgress(qint64,qint64)), this, SIGNAL(progress(qint64,qint64)));
    disconnect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));

    if (isAborted)
    {
        emit finished(false, true, QString());
    }
    else if (reply->error()>0)
    {
        emit finished(true, false, tr("Network error: %1").arg(QString::number(reply->error())));
    }
    else
    {
        QString loc = reply->rawHeader("filehash");
        if (!loc.isEmpty())
            emit finished(false, false, siteurl+loc);
        else
            emit finished(true, false, tr("Error: %1").arg(reply->errorString()));
    }
}
