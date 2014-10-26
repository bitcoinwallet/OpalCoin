#include "resultdialog.h"
#include "ui_resultdialog.h"
#include <QClipboard>

ResultDialog::ResultDialog(QWidget *parent) :
    QDialog(parent),
    m_ui(new Ui::ResultDialog)
{
    m_ui->setupUi(this);
    connect(m_ui->pushButtonCopy, SIGNAL(clicked()), this, SLOT(copyPressed()));
}

ResultDialog::~ResultDialog()
{
    delete m_ui;
}

void ResultDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        m_ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

void ResultDialog::showWithText(QStringList lines)
{
    if (!lines.isEmpty())
    {
        m_ui->textEdit->setPlainText(lines.join("\n"));
        show();
    }
}

void ResultDialog::copyPressed()
{
    qApp->clipboard()->setText(m_ui->textEdit->toPlainText());
}
