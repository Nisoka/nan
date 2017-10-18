#include "qlogin.h"

QLogin::QLogin(QWidget *parent)
    :QWidget(parent)
    ,progressCnt(0)
{

    this->setWindowModality(Qt::ApplicationModal);
    setMinimumSize(550 , 160);
    setMaximumSize(550 , 160);
    setWindowFlags(Qt::FramelessWindowHint);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(58 , 18 , 218)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);


    m_pWidget = new QWidget(this);
    m_pWidget->setGeometry(5 , 5 , 540 , 150);
    QPalette pal2(m_pWidget->palette());

    pal2.setColor(QPalette::Background, QColor(255 , 255 , 255)); //设置背景黑色
    m_pWidget->setAutoFillBackground(true);
    m_pWidget->setPalette(pal2);
    m_pWidget->show();


    labelInfo = new QLabel(this);
    labelInfo->setText(tr("Initializing..."));
    labelInfo->setGeometry(QRect(10 , 10 , 500 , 50));
    progressBar = new QProgressBar(this);
    progressBar->setGeometry(QRect(120 , 100 , 400 , 30));
    timer = new QTimer(this);

    QObject::connect(timer , SIGNAL(timeout()) , this , SLOT(waitting()));
}


QLogin::~QLogin()
{

}

void QLogin::startLogin()
{
    timer->setInterval(200);
    timer->start();
}

void QLogin::waitting()
{
    progressCnt ++;
    progressBar->setValue(progressCnt);
    if(progressCnt == 100)
    {
        emit LoginOver();
        this->hide();
    }

}



QAbout::QAbout(QWidget *parent)
    :QWidget(parent)
{

    this->setWindowModality(Qt::ApplicationModal);
    setMinimumSize(550 , 160);
    setMaximumSize(550 , 160);
    setWindowFlags(Qt::FramelessWindowHint);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(58 , 18 , 218)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);


    m_pWidget = new QWidget(this);
    m_pWidget->setGeometry(5 , 5 , 540 , 150);

    QPalette pal2(m_pWidget->palette());
    pal2.setColor(QPalette::Background, QColor(255 , 255 , 255)); //设置背景黑色
    m_pWidget->setAutoFillBackground(true);
    m_pWidget->setPalette(pal2);
    m_pWidget->show();

    btnConfirm = new QPushButton(this);
    btnConfirm->move(450 , 120);
//    btnConfirm->setFlat(true);
    btnConfirm->setText(tr("OK"));

    labelInfo1 = new QLabel(this);
    labelInfo2 = new QLabel(this);
    labelInfo3 = new QLabel(this);

    labelVersionFirmware = new QLabel(this);
    labelVersionHardWare = new QLabel(this);
    labelVersionQAM = new QLabel(this);

    labelInfo1->setGeometry(70 , 20 , 200 , 30);
    labelInfo2->setGeometry(70 , 70 , 200 , 30);
    labelInfo3->setGeometry(70 , 120 , 200 , 30);

    labelVersionFirmware->setGeometry(290 , 20 , 150 , 30);
    labelVersionQAM->setGeometry(290 , 70 , 150 , 30);
    labelVersionHardWare->setGeometry(290 , 120 , 150 , 30);

    labelInfo1->setText(tr("FirmWare Version:"));
    labelInfo1->setAlignment(Qt::AlignRight);
    labelInfo2->setText(tr("QAM Version:"));
    labelInfo2->setAlignment(Qt::AlignRight);
    labelInfo3->setText(tr("HardWare Version:"));
    labelInfo3->setAlignment(Qt::AlignRight);



    labelVersionFirmware->setText("0.0.0");
    labelVersionFirmware->setAlignment(Qt::AlignLeft);

    labelVersionHardWare->setText("0.0.0");
    labelVersionHardWare->setAlignment(Qt::AlignLeft);

    char strVersion[20] = "";
    sprintf(strVersion , "%d.%02d.%02d" , QAM_VERSION_1 , QAM_VERSION_2 , QAM_VERSION_3);
    labelVersionQAM->setText(QString(strVersion));
    labelVersionQAM->setAlignment(Qt::AlignLeft);

    QObject::connect(btnConfirm , SIGNAL(clicked(bool)) , this , SLOT(hide()));
}

QAbout::~QAbout()
{

}

void QAbout::setFirmwareVersion(int Version1 , int Version2 , int Version3)
{
    char strVersion[20] = "";
    sprintf(strVersion , "%d.%02d.%02d" , Version1 , Version2 , Version3);
    labelVersionFirmware->setText(QString(strVersion));
}

void QAbout::setHardWareVersion(int Version1, int Version2, int Version3)
{
    char strVersion[20] = "";
    sprintf(strVersion , "%d.%02d.%02d" , Version1 , Version2 , Version3);
    labelVersionHardWare->setText(QString(strVersion));
}


QInfoBox::QInfoBox(QWidget *parent)
    :QWidget(parent)
{

    this->setWindowModality(Qt::ApplicationModal);
    setMinimumSize(300, 100);
    setMaximumSize(300, 100);
    setWindowFlags(Qt::FramelessWindowHint);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(58 , 18 , 218)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);


    m_pWidget = new QWidget(this);
    m_pWidget->setGeometry(5 , 5 , 290, 90);

    QPalette pal2(m_pWidget->palette());
    pal2.setColor(QPalette::Background, QColor(255 , 255 , 255)); 
    m_pWidget->setAutoFillBackground(true);
    m_pWidget->setPalette(pal2);
    m_pWidget->show();


    labelInfo1 = new QLabel(this);
    labelInfo2 = new QLabel(this);

    labelInfo1->setGeometry(0 , 0 , 290 , 60);

    labelInfo1->setAlignment(Qt::AlignCenter);
    labelInfo2->setAlignment(Qt::AlignRight);

    btnConfirm = new QPushButton(this);
    btnConfirm->move(130 , 60);
    btnConfirm->setText(tr("OK"));

    btnCancel = new QPushButton(this);
    btnCancel->move(210 , 60);
    btnCancel->setText(tr("Cancel"));

    QObject::connect(btnCancel , SIGNAL(clicked(bool)) , this , SLOT(hide()));
    QObject::connect(btnConfirm, SIGNAL(clicked(bool)), this, SLOT(slotConfirm()));
    QObject::connect(btnConfirm , SIGNAL(clicked(bool)) , this , SLOT(hide()));
}

QInfoBox::~QInfoBox()
{

}

void QInfoBox::SetShowInfo(QString strInfo)
{
  labelInfo1->setText(strInfo);
}

void QInfoBox::slotConfirm()
{
  emit signalConfirm();
}

