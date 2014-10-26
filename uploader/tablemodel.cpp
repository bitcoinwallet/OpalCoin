#include "tablemodel.h"
#include <QFile>
#include <QFileInfo>
#include <QMessageBox>

TableModel::TableModel(QObject *parent) : QAbstractTableModel(parent)
{
    uploader = new UpLoader(this);

    iconNew = new QIcon(":/icons/status/new.png");
    iconInProgress = new QIcon(":/icons/status/wait.png");
    iconDone = new QIcon(":/icons/status/done.png");
    iconError = new QIcon(":/icons/status/error.png");
    iconAborted = new QIcon(":/icons/status/aborted.png");

    connect(uploader, SIGNAL(progress(qint64,qint64)), this, SIGNAL(progress(qint64,qint64)));
    connect(uploader, SIGNAL(started()), this, SLOT(uploadStarted()));
    connect(uploader, SIGNAL(finished(bool,bool,QString)), this, SLOT(uploadFinished(bool,bool,QString)));
}

TableModel::~TableModel()
{
    delete iconNew;
    delete iconInProgress;
    delete iconDone;
    delete iconError;
    delete iconAborted;
}

int TableModel::rowCount(const QModelIndex &) const
{
    return list.size();
}
int TableModel::columnCount(const QModelIndex &) const
{
    return 4;
}
QVariant TableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();
    if (index.row()>=list.size())
        return QVariant();
    if (role==Qt::DecorationRole)
    {
        if (index.column()==2)
            return *statusToIcon(list.at(index.row()).status);
    }
    if (role!=Qt::DisplayRole)
        return QVariant();

    switch (index.column())
    {
        case 0:
            return QFileInfo(list.at(index.row()).filename).fileName();
            break;
        case 1:
            return bytesToSize(list.at(index.row()).size);
            break;
        case 2:
            return statusToStr(list.at(index.row()).status);
            break;
        case 3:
            return list.at(index.row()).link;
    }
    return QVariant();
}

QVariant TableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (role!=Qt::DisplayRole)
        return QVariant();
    if (orientation!=Qt::Horizontal)
        return QVariant();
    switch (section)
    {
        case 0:
            return tr("Filename");
            break;
        case 1:
            return tr("Size");
            break;
        case 2:
            return tr("Status");
            break;
        case 3:
            return tr("Link to file");
            break;
    }
    return QVariant();
}

void TableModel::setSettings(SETTINGS *settings)
{
    this->settings = settings;
}

//////////////////////////////
void TableModel::addFile(const QString &path)
{
    beginInsertRows(QModelIndex(), list.size(), list.size());
    TableItem item;
    item.filename = path;
    QFile file(path);
    item.size = file.size();
    item.status = StatusNew;
    list << item;
    endInsertRows();
}
void TableModel::deleteFile(int index)
{
    beginRemoveRows(QModelIndex(), index, index);
    list.removeAt(index);
    endRemoveRows();
}

QStringList TableModel::urlsList()
{
    QStringList result;
    for (int i=0; i<list.size(); i++)
    {
        QString url = list[i].link;
        if (url.indexOf("http://", 0)>=0)
        {
            result << url;
        }
    }
    return result;
}

bool TableModel::readFromFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly))
        return false;
    QDataStream in(&file);
    in.setVersion(QDataStream::Qt_4_5);
    int i=0;
    while (!in.atEnd())
    {
        TableItem item;
        qint16 statusint;
        in >> item.filename >> item.size >> statusint >> item.link;
        item.status = (UploadStatus)statusint;
        list << item;
        i++;
    }
    emit dataChanged(QModelIndex(), QModelIndex());
    return true;
}
bool TableModel::writeToFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly))
        return false;
    QDataStream out(&file);
    out.setVersion(QDataStream::Qt_4_5);
    for (int i=0; i<list.size(); ++i)
    {
        out << list[i].filename << list[i].size << qint16(list[i].status) << list[i].link;
    }
    return true;
}

/////////////////////////////////////////public slots://////////////////////////////////
void TableModel::clear()
{
    beginRemoveRows(QModelIndex(), 0, list.size()-1);
    list.clear();
    endRemoveRows();
}

void TableModel::startUploading()
{
    if (list.size()>0)
    {
        currentIndex = 0;
        while ((currentIndex<list.size()) && (list[currentIndex].status==StatusDone))
            currentIndex++;
        if (currentIndex>=list.size())
            return;

        emit allUploadsStarted();
            list[currentIndex].status = StatusInProgress;
            emit dataChanged(this->index(currentIndex, 2), this->index(currentIndex, 2));
            uploader->upload(list[currentIndex].filename, settings->passForDown, settings->comment);
    }
}
void TableModel::stopUploading()
{
    uploader->abort();
}
//////////////////////////////////////////private//////////////////////////////////////////
QIcon *TableModel::statusToIcon(UploadStatus status) const
{
    switch (status)
    {
        case StatusNew:
            return iconNew;
        case StatusInProgress:
            return iconInProgress;
        case StatusDone:
            return iconDone;
        case StatusError:
            return iconError;
        case StatusAborted:
            return iconAborted;
    }
    return 0;
}
//////////////////////////////////////private slots:////////////////////////////////////////
void TableModel::uploadStarted()
{
    emit statusChanged(tr("Uploading..."));
    emit uploadingStarted();
}

void TableModel::uploadFinished(bool error, bool aborted, const QString &text)
{
    if (aborted)
    {
        emit statusChanged(tr("File %1 aborted").arg(list[currentIndex].filename));
        list[currentIndex].status = StatusAborted;
        emit dataChanged(this->index(currentIndex, 2), this->index(currentIndex, 2));
        emit allUploadsFinished();
        return;
    }
    if (error)
    {
        emit statusChanged(tr("Error uploading file %1: %2").arg(list[currentIndex].filename).arg(text));
        list[currentIndex].status = StatusError;
        emit dataChanged(this->index(currentIndex, 2), this->index(currentIndex, 2));
    }
    else
    {
        emit statusChanged(tr("File %1 successfully uploaded").arg(list[currentIndex].filename));
        list[currentIndex].status = StatusDone;
        emit dataChanged(this->index(currentIndex, 2), this->index(currentIndex, 2));
    }
    list[currentIndex].link = text;
    emit dataChanged(this->index(currentIndex, 3), this->index(currentIndex, 3));
    ////////////////////////////////////////////////////////////////////////////////////
    do
    {
        currentIndex++;
    } while (currentIndex<list.size() && list[currentIndex].status==StatusDone);
    if (currentIndex<list.size())
    {
        list[currentIndex].status = StatusInProgress;
        emit dataChanged(this->index(currentIndex, 2), this->index(currentIndex, 2));
        uploader->upload(list[currentIndex].filename, settings->passForDown, settings->comment);
    }
    else
    {
        emit statusChanged(tr("All uploads finished"));
        emit allUploadsFinished();
    }
}
