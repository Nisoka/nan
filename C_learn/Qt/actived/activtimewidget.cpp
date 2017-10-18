#include "activtimewidget.h"
#include "ui_activtimewidget.h"

#include <fcntl.h>

char trText_Active[100][300];

int LoadTranslateText( QString dir, char text[][300])
{
    FILE* pFile = NULL;
    int counter = 0;

    char *str;

    QByteArray ba = dir.toLatin1();

    str=ba.data();



    if((pFile = fopen(str, "r")) != NULL)
    {

        while(fgets(text[counter], sizeof(text[counter]), pFile) != NULL)
        {
#ifndef QT_CREATOR_DEBUG
            text[counter][strlen(text[counter]) - 2] = 0;
#else
         // text[counter][strlen(text[counter]) - 1] = 0;// 原来是 text[counter]) - 2
#endif
            counter++;

        }
        fclose(pFile);
        system("sync");
        return 0;

    }
    else
    {

        return -1;
    }
}

int SetCurrentTranslateText(int LanguageFlag, QString dirPath)
{
    memset(trText_Active, 0, sizeof(trText_Active));
    if(LanguageFlag == Chinese_Active)
    {
        if(LoadTranslateText(dirPath +LANGUAGE_CHI_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath +LANGUAGE_CHI_PATH_ACTIVE, trText_Active);
        }

    }
#if 0
    else if(LanguageFlag == French_Active)
    {
        if(LoadTranslateText(dirPath +LANGUAGE_FRE_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_FRE_PATH_ACTIVE, trText_Active);
        }

    }
    else if(LanguageFlag == Spanish_Active)
    {
        if(LoadTranslateText(dirPath +LANGUAGE_SPA_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath +LANGUAGE_SPA_PATH_ACTIVE, trText_Active);
        }

    }
    else if(LanguageFlag == Portuguese_Active)
    {
        if(LoadTranslateText(dirPath + LANGUAGE_POR_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_POR_PATH_ACTIVE, trText_Active);
        }

    }
    else if(LanguageFlag == Russian_Active)
    {
        if(LoadTranslateText(dirPath + LANGUAGE_RUS_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_RUS_PATH_ACTIVE, trText_Active);
        }

    }
    else if(LanguageFlag == Italian_Active)
    {
        if(LoadTranslateText(dirPath + LANGUAGE_ITA_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_ITA_PATH_ACTIVE, trText_Active);
        }

    }
    else if(LanguageFlag == German_Active)
    {
        if(LoadTranslateText(dirPath + LANGUAGE_GER_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_GER_PATH_ACTIVE, trText_Active);
        }
    }
    else if(LanguageFlag == ChineseT_Active)
    {
        printf(" == ChineseT !\n");
        if(LoadTranslateText(dirPath + LANGUAGE_CNT_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_CNT_PATH_ACTIVE, trText_Active);
            printf(" Load ChineseT OK!\n");
        }
    }
    else if(LanguageFlag == Japanese_Active)
    {
        if(LoadTranslateText(dirPath + LANGUAGE_JPN_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_JPN_PATH_ACTIVE, trText_Active);
        }
    }
    else if(LanguageFlag == Korean_Active)
    {
        if(LoadTranslateText(dirPath + LANGUAGE_KOR_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_KOR_PATH_ACTIVE, trText_Active);
        }
    }
    else if(LanguageFlag == Czech_Active)
    {
        if(LoadTranslateText(dirPath + LANGUAGE_CZE_PATH_ACTIVE, trText_Active) == -1)
        {
            LoadTranslateText(dirPath + LANGUAGE_CZE_PATH_ACTIVE, trText_Active);
        }
    }
#endif
    else
    {
        QString temp = dirPath + LANGUAGE_ENG_PATH_ACTIVE;
        //printf("%s  =================== \n", temp.toStdString().c_str());
        LoadTranslateText(dirPath + LANGUAGE_ENG_PATH_ACTIVE, trText_Active);
    }

    return 0;
}

int myMessageBox(QString text)
{
    QMessageBox msgBox;
    msgBox.setText(text);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/info.ico"));
    if(msgBox.exec() == QMessageBox::Ok)
        return -1;

    return 0;
}

void ReadE2prom(int addr , char *buf, int size)
{
    int fd = ::open("/dev/fc-24lc02",O_RDWR);
    if(fd == -1)
    {
        printf("open /dev/fc-24lc02  failed!\n");
        return;
    }

    lseek(fd, addr, SEEK_SET);

//    write(fd,buf,4);
    ::read(fd, buf, size);

    ::close(fd);
}

void WriteE2prom(int addr , char *buf, int size)
{
    int fd = ::open("/dev/fc-24lc02",O_RDWR);
    if(fd == -1)
    {
        printf("open /dev/fc-24lc02  failed!\n");
        return;
    }

    lseek(fd, addr, SEEK_SET);

    ::write(fd, buf, size);
//    read(fd, buf, size);

    ::close(fd);
}


ActivTimeWidget::ActivTimeWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ActivTimeWidget)
{
    ui->setupUi(this);

    QPalette palette;
    palette.setBrush(QPalette::Background, QBrush(QColor(Qt::white)));
    this->setAutoFillBackground(true);
    this->setPalette(palette);

}

ActivTimeWidget::~ActivTimeWidget()
{
    delete ui;
}

void ActivTimeWidget::setActivState(ActivState state)
{

//    int cnt = 0;
//    while(cnt < 7){
//        printf(trText_Active[cnt++]);
//        printf(" \n");
//    }

    ui->label_Info->setFont(QApplicationFont);
    ui->pushButton_Buy->setFont(QApplicationFont);
    ui->pushButton_Try->setFont(QApplicationFont);
    ui->lineEdit_Key->setFont(QApplicationFont);
    ui->label_Key->setFont(QApplicationFont);

    ui->label_Info->setText(trText_Active[state]);
    ui->label_Info->setWordWrap(true);

    ui->pushButton_Buy->setText(trText_Active[5]);
    ui->label_Key->setText(trText_Active[8]);
    ui->pushButton_Cancel->setText(trText_Active[7]);

    isCancel = true;
    switch(state){
    case stateMouthIn:
        ui->pushButton_Try->setText(trText_Active[6]);
        isCancel = false;
        break;
    case stateMouthOut:
        ui->pushButton_Try->setText(trText_Active[7]);
        break;
    case stateYearIn:
        break;
    case stateYearOut:
        ui->pushButton_Try->setText(trText_Active[7]);
        break;
    case stateAll:
        break;
//        ui->label_Info
    }
}

void ActivTimeWidget::paintEvent(QPaintEvent *event)
{
    QPainter *painter = new QPainter(this);
    QPen pen;
    pen.setColor(Qt::blue);
    pen.setWidth(3);
    painter->setPen(pen);
    painter->drawLine(QPoint(0, 1), QPoint(this->width(), 1));
    painter->drawLine(QPoint(0, this->height() - 2), QPoint(this->width(), this->height()- 2));
    painter->drawLine(QPoint(1, 0), QPoint(1, this->height() ));
    painter->drawLine(QPoint(this->width() - 2, 0), QPoint(this->width() - 2, this->height()));

    delete painter;
}

void ActivTimeWidget::on_pushButton_Buy_clicked()
{
    QString strKey = ui->lineEdit_Key->text();
    emit signalUserBuyTheKey(strKey);
}

void ActivTimeWidget::on_pushButton_Try_clicked()
{
    if(isCancel)
        emit signalUserCancel();
    else
        emit signalUserTryIt();
}

void ActivTimeWidget::on_pushButton_Cancel_clicked()
{
    emit signalUserCancel();
}
