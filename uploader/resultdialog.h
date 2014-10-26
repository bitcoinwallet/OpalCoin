#ifndef RESULTDIALOG_H
#define RESULTDIALOG_H

#include <QDialog>

namespace Ui {
    class ResultDialog;
}

class ResultDialog : public QDialog {
    Q_OBJECT
public:
    ResultDialog(QWidget *parent = 0);
    ~ResultDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::ResultDialog *m_ui;

public slots:
    void showWithText(QStringList lines);
private slots:
    void copyPressed();
};

#endif // RESULTDIALOG_H
