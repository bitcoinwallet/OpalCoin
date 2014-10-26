#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "settingsdialog.h"
#include "tablemodel.h"
#include <QSystemTrayIcon>
#include <QTime>
#include <QCloseEvent>
#include <QMenu>
#include "resultdialog.h"

namespace Ui {
class FileWindowClass;
}
class ClientModel;

class FileWindow : public QWidget
{
    Q_OBJECT

public:
    explicit FileWindow(QWidget *parent = 0);
    ~FileWindow();

    void setModel(ClientModel *model);

private:
    Ui::FileWindowClass *ui;
    SettingsDialog *settingsdialog;
    SETTINGS settings;
    TableModel *tablemodel;
    QSystemTrayIcon *trayicon;
    QMenu *traymenu;
    QTime *time;
    ResultDialog *resdialog;
    QString dialogdir;
    void readSettings();
    void writeSettings();
    void closeEvent(QCloseEvent *event);
    bool okToExit();
    QString listfile;
    void setListFile();
    ClientModel *model;

public slots:


private slots:
    void uploadProgress(qint64 bytesSent, qint64 bytesTotal);
    void allUploadsStarted();
    void allUploadsFinished();
    void settingsPressed();
    void settingsChanged(SETTINGS sets);
    void addPressed();
    void deletePressed();
    void uploaderStarted();
    void trayIconActivated(QSystemTrayIcon::ActivationReason reason);
    void exitApplication();
    void showLinks();
    void aboutPressed();
};

#endif // MAINWINDOW_H
