#ifndef GLOBAL_H
#define GLOBAL_H
#include <QObject>
#include <QString>

const QString version = "0.2 beta";

enum UploadStatus
{
    StatusNew = 0,
    StatusInProgress = 1,
    StatusDone = 2,
    StatusError = 3,
    StatusAborted = 4
};

struct SETTINGS
{
    bool authentification;
    QString email, password;

    QString passForDown;
    QString comment;

    bool trayicon, mintotray;
};

QString bytesToSize(qint64 size);
QString statusToStr(UploadStatus status);

//qint16 statusToInt(UploadStatus status);
//UploadStatus intToStatus(qint16 intstat);
#endif // GLOBAL_H
