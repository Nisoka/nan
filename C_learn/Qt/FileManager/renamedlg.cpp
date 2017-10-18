#include "renamedlg.h"
#include "ui_renamedlg.h"



RenameDlg::RenameDlg(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::RenameDlg)
{
    ui->setupUi(this);
    QRegExp rx("[^\\\\/:*?\"<>|]*");
    QValidator *validator = new QRegExpValidator(rx, this);
    ui->editName->setValidator(validator);
    connect(ui->okBtn,SIGNAL(clicked()),this,SLOT(okBtnClicked()));
    connect(ui->cancelBtn,SIGNAL(clicked()),this,SLOT(cancelBtnClicked()));    
}

RenameDlg::~RenameDlg()
{
    delete ui;
}

void RenameDlg::showEvent(QShowEvent *event)
{
    ui->editName->setFocus();
    QDialog::showEvent(event);
}

void RenameDlg::setFileName(QString name)
{
    ui->editName->setText(name);
}

void RenameDlg::okBtnClicked()
{
    if(ui->editName->text() == "." ||
            ui->editName->text() == "..")
    {
        ui->editName->clear();
        return;
    }
    m_newName = ui->editName->text();
    ui->editName->clear();
    accept();
#ifndef PC_DEBUG
    //added by zbs
    QCopChannel::send("fcdesk", "IME", "HIDE");
#endif
}

void RenameDlg::cancelBtnClicked()
{
    ui->editName->clear();
    reject();
#ifndef PC_DEBUG
    //added by zbs
    QCopChannel::send("fcdesk", "IME", "HIDE");
#endif
}

QString RenameDlg::getFileName()
{
    return m_newName;
}







