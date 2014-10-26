#include "global.h"

QString bytesToSize(qint64 size)
{
    if (size < 0)
        return QString();
    if (size < 1024)
        return QString::number(((double)size), 'f', 2).append(QObject::tr(" B"));
    if ((size >= 1024) && (size < 1048576))
        return QString::number(((double)size)/1024, 'f', 2).append(QObject::tr(" KB"));
    if ((size >= 1048576) && (size < 1073741824))
        return QString::number(((double)size)/1048576, 'f', 2).append(QObject::tr(" MB"));
    if (size >=1073741824)
        return QString::number(((double)size)/1073741824, 'f', 2).append(QObject::tr(" GB"));
    return QString();
}

QString statusToStr(UploadStatus status)
{
    switch (status)
    {
        case StatusNew:
            return QObject::tr("New");
            break;
        case StatusInProgress:
            return QObject::tr("Uploading");
            break;
        case StatusDone:
            return QObject::tr("Ready");
            break;
        case StatusError:
            return QObject::tr("Error");
            break;
        case StatusAborted:
            return QObject::tr("Aborted");
            break;
    }
    return QString();
}
