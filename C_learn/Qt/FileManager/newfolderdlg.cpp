#include "newfolderdlg.h"
#include "ui_newfolderdlg.h"


NewFolderDlg::NewFolderDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NewFolderDlg)
{
    ui->setupUi(this);
    QRegExp rx("^[^.][^\\\\/:*?\"<>|]*");
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->editFolder->setValidator(validator);

    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtnClicked()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtnClicked()));    
}

NewFolderDlg::~NewFolderDlg()
{
    delete ui;
}

void NewFolderDlg::showEvent(QShowEvent *event)
{
    ui->editFolder->setFocus();
    QDialog::showEvent(event);
}

QString NewFolderDlg::getFolderName()
{
    return m_folderName;
}

void NewFolderDlg::okBtnClicked()
{
    if(ui->editFolder->text() == "." ||
            ui->editFolder->text() == "..")
    {
        ui->editFolder->clear();
        return;
    }
    m_folderName = ui->editFolder->text();
    ui->editFolder->clear();
    accept();
#ifndef PC_DEBUG
    //added by zbs
    QCopChannel::send("fcdesk", "IME", "HIDE");
#endif
}

void NewFolderDlg::cancelBtnClicked()
{
//    done(-1);

#ifndef PC_DEBUG
    //added by zbs
    QCopChannel::send("fcdesk", "IME", "HIDE");
#endif

    reject();
}
