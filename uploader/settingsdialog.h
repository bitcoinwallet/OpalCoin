#ifndef SETTINGSDIALOG_H
#define SETTINGSDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui {
    class SettingsDialog;
}

class SettingsDialog : public QDialog {
    Q_OBJECT
public:
    SettingsDialog(QWidget *parent = 0);
    ~SettingsDialog();
    void showDialog(SETTINGS sets);

protected:
    void changeEvent(QEvent *e);

private:
    Ui::SettingsDialog *m_ui;

private slots:
    void dialogAccepted();

signals:
    void settingsChanged(SETTINGS sets);
};

#endif // SETTINGSDIALOG_H
