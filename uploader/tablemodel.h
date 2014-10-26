#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include <QAbstractTableModel>
#include <QList>
#include "uploader.h"
#include "settingsdialog.h"
#include <QIcon>
#include "global.h"

struct TableItem
{
    QString filename;
    qint64 size;
    UploadStatus status;
    QString link;
};

class TableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    ////////////////////////////////////////////////////////////////////////////////////////////////
    TableModel(QObject *parent = 0);
    ~TableModel();
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    ////////////////////////////////////////////////////////////////////////////////////////////////
    void setSettings(SETTINGS *settings);

    void addFile(const QString &path);
    void deleteFile(int index);
    QStringList urlsList();
    bool readFromFile(const QString &filename);
    bool writeToFile(const QString &filename);

public slots:
    void clear();
    void startUploading();
    void stopUploading();

private:
    QList<TableItem> list;
    UpLoader *uploader;
    SETTINGS *settings;
    int currentIndex;
    QIcon *iconNew, *iconInProgress, *iconDone, *iconError, *iconAborted;
    QIcon *statusToIcon(UploadStatus status) const;

private slots:
    void uploadStarted();
    void uploadFinished(bool error, bool aborted, const QString &text);

signals:
    void statusChanged(const QString &status);
    void progress(qint64 bytesSent, qint64 bytesTotal);
    void uploadingStarted();
    void allUploadsStarted();
    void allUploadsFinished();
};

#endif // TABLEMODEL_H
