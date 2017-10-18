#include "activationcode.h"
#include "ui_activationcode.h"
#include <QTimer>
#include <QPixmap>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdio.h>

ActivationCode::ActivationCode(QString name, QString dirPath, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActivationCode)
{
    this->name = name;
    ui->setupUi(this);
    timeActivWidget = NULL;

    this->appPath = dirPath;
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setModal(true);

    //judge if the file exits
    QFile file(dirPath + "/" + "hasActived");
    if(file.exists())   //if file exists, return go on status
    {
        qDebug("file exists !\n");
        resultSta = FILE_EXIST;        
        QTimer::singleShot(1, this, SLOT(dealFileExists()));
        this->show();
    }
    else                //if file do not exits, show the widow
    {
        qDebug("file not exists !\n");

        QPalette palette;
        QPixmap map(dirPath + "/" + "activationCode.png");
        palette.setBrush(QPalette::Background, QBrush(map));
        this->setAutoFillBackground(true);
        this->setPalette(palette);

        ui->resultTip->hide();
        ui->tips->setText("Please input the activation code :");
        ui->tips->show();
        this->show();
    }



}

ActivationCode::~ActivationCode()
{
    delete ui;
}

void ActivationCode::dealFileExists()
{
    if(timeActivWidget == NULL)
        emit close();
}




void ActivationCode::createHasActivedFile(QString appPath)
{
    qDebug("createHasActivedFile");
    qDebug("appPath = %s\n", qPrintable(appPath));

    QString path = appPath + "/hasActived";
    int fd = ::open(path.toLatin1(), O_CREAT);
    ::close(fd);
}


void ActivationCode::on_buttonBox_accepted()
{
    //get activate code user input
    usrCode = ui->inputEdt->text();

    //caculate real active code from name and sn
    sn = getSN();
    actCode = getActivationCode(name, sn);

    //compare real active code with user input code
    if(!usrCode.compare(usrCode, actCode))  //success to activate
    {        
        //create the file
        createHasActivedFile(appPath);
        //return go on status
        resultSta = ACTIVE_SUCCESS;
        emit close();
    }
    else
    {
        //faile to activate
        ui->resultTip->setText("Failed to activate !");
        ui->resultTip->show();
    }
}

void ActivationCode::on_buttonBox_rejected()
{
    resultSta = ACTIVE_FAILED;
    emit close();
}

QString ActivationCode::getSN(void)
{
    char buf[30] = {0};
    int fd = ::open("/dev/fc-24lc02",O_RDWR);
    if(fd == -1)
    {
        return "";
    }
    lseek(fd,6,SEEK_SET);
    int size = read(fd,buf,16);

    QString result(buf);
    sn = result;
    ::close(fd);

    return sn;
}

QString ActivationCode::rerangeCodeOrder(QString code)
{
    char ch;
    char buf[255] = {0};
    int len = code.length();
    char rerangeIndex[3][2] = {{1,3}, {0,4}, {2,5}};

    //change QString to char array
    memcpy(buf, code.toLatin1().data(), len);

    //rerange the array
    for(int i=0; i<len/2; i++)
    {
        ch = buf[rerangeIndex[i][0]];
        buf[rerangeIndex[i][0]] = buf[rerangeIndex[i][1]];
        buf[rerangeIndex[i][1]] = ch;
    }

    //change char array to QString
    QString codeS = QLatin1String(buf);

    return codeS;
}

QString md5(const QString strSrc)
{
    QString md5;
    QByteArray ba,bb;
    QCryptographicHash md(QCryptographicHash::Md5);
    ba.append(strSrc);
    md.addData(ba);
    bb = md.result();
    md5.append(bb.toHex());

//    qDebug("md5 = %s\n", md5.toLatin1().data());
    return md5;
}

//基于短链接算法
QStringList ShortUrl(QString url)
{
    char czBase[] = {'a' , 'b' , 'c' , 'd' , 'e' , 'f' , 'g' , 'h' ,
        'i' , 'j' , 'k' , 'l' , 'm' , 'n' , 'o' , 'p' , 'q' , 'r' , 's' , 't' ,
        'u' , 'v' , 'w' , 'x' , 'y' , 'z' , '0' , '1' , '2' , '3' , '4' , '5' ,
        '6' , '7' , '8' , '9' , 'A' , 'B' , 'C' , 'D' , 'E' , 'F' , 'G' , 'H' ,
        'I' , 'J' , 'K' , 'L' , 'M' , 'N' , 'O' , 'P' , 'Q' , 'R' , 'S' , 'T' ,
        'U' , 'V' , 'W' , 'X' , 'Y' , 'Z'};

    QString value = md5(url);
    QStringList results ;
    for (int i = 0; i < value.length()/8; i++)
    {
        QString tempResult = value.mid(i*8,8);
        long long lHexLong = 0x3FFFFFFF & tempResult.toLongLong(0, 16);

        QString outchar = "";
        for (int j = 0; j < 6; j++)
        {
            long index = 0x0000003D & lHexLong;
            outchar += czBase[(int)index];
            lHexLong = lHexLong >> 5;
        }
        results.append(outchar.toLower());
    }
    return results;
}

QString ActivationCode::getActivationCode(QString name, QString sn)
{
    //use sn+name as the key
    QString keyStr = sn + name;
    qDebug("keyStr = %s", keyStr.toLatin1().data());

    QStringList code = ShortUrl(keyStr);
    actCode = rerangeCodeOrder(code.at(0));
    return actCode;
}

QString ActivationCode::cutOffEqualChar(QString str)
{
    char buf[255] = {0};
    int len = str.length();

    memcpy(buf, str.toLatin1().data(), len);
    for(int i=len-1; i>=0; i--)
    {
        if(buf[i] != '=')
            break;
        else
            buf[i] = '\0';
    }

    QString codeS = QLatin1String(buf);

    return codeS;
}

void ActivationCode::paintEvent(QPaintEvent *)
{

}

void ActivationCode::hideEvent(QHideEvent *)
{
    resultSta = ACTIVE_FAILED;
    emit close();
}

#define MODULES_COUNT 14
#define MODULES_BASE_ADDR 128
int ActivationCode::currentModuleIndex()
{
    QString modules[MODULES_COUNT] = {"VFL",
                            "OPM",
                            "Laser",
                            "FiberSpot",
                            "FiberPath",
                            "FiberTune",
                            "QAM",
                            "TDR",
                            "Remote",
                            "Worker",
                            "Seeker",
                            "WiFi",
                            "GPS",
                            "Bluetooth"};

    int index = 0;
    do{
        if(name == modules[index]){
            return MODULES_BASE_ADDR + index*25;
        }
        index ++;
    }while(index < MODULES_COUNT);

    return 0;
}


void ActivationCode::getCurrentModuleTime()
{
    char *buf;
    ActivBuyState tempActState;
    buf = (char *)&tempActState;
    actAddr = currentModuleIndex();
    if(actAddr == 0){
        printf("AAAAAAAAAAAAAAAAAAA \n can not recognise  the model name!! \n ");
        buyMode = BUYMODEMOUT;
        dayCount = 100;

    }else{
        ReadE2prom(actAddr, buf, sizeof(ActivBuyState));

        buyMode = tempActState.buyMode;
        dayCount = buf[4] << 24 | buf[3] << 16 | buf[2] << 8 | buf[1];

        if(tempActState.activeIndex == 0xFF)
            tempActState.activeIndex = 0;

        activeIndex = tempActState.activeIndex;

        printf("Buy Mode is %d, day Count left is %d , ActiveIndex is %d\n", buyMode, dayCount, activeIndex);
        if(buyMode != BUYMODEMOUT && buyMode != BUYMODEYEAR && buyMode != BUYMODELIFE){
            clearUserBuy();
        }
    }
}

void ActivationCode::checkActivTimeState()
{
    if(buyMode == BUYMODEMOUT){
        timeActivWidget->setActivState(stateMouthIn);
        if(dayCount > 31){
            timeActivWidget->setActivState(stateMouthOut);
        }
    }else if(buyMode == BUYMODEYEAR){
        timeActivWidget->setActivState(stateYearIn);
        if(dayCount > 366){
            timeActivWidget->setActivState(stateYearOut);
        }else{
            isInTheBuy = true;
        }
    }else if(buyMode == BUYMODELIFE){
        timeActivWidget->setActivState(stateAll);
        isInTheBuy = true;
    }
}

void ActivationCode::useTimeActive(int language, QString path)
{

    SetCurrentTranslateText(language, path);
    timeActivWidget = new ActivTimeWidget(this);
    timeActivWidget->setGeometry(this->rect());
    timeActivWidget->hide();

    connect(timeActivWidget, SIGNAL(signalUserCancel()), this, SLOT(userCancel()));
    connect(timeActivWidget, SIGNAL(signalUserTryIt()), this, SLOT(userTryIt()));
    connect(timeActivWidget, SIGNAL(signalUserBuyTheKey(QString)), this, SLOT(userBuyTheKey(QString)));


//    clearUserBuy();
    isInTheBuy = false;
    QTimer::singleShot(1, this, SLOT(userIntheBuy()));
    getCurrentModuleTime();
    checkActivTimeState();
    timeActivWidget->show();
}


void ActivationCode::userCancel()
{
    printf("User Cancel !!! \n");
    resultSta = ACTIVE_FAILED;
    emit close();
}

void ActivationCode::userTryIt()
{
    printf("User Try It!! \n");
    resultSta = ACTIVE_SUCCESS;
    emit close();
}

void ActivationCode::clearUserBuy()
{
    char *buf;
    ActivBuyState tempActState;
    buf = (char *)&tempActState;
    memset(buf, '\0', sizeof(tempActState));

    tempActState.buyMode = BUYMODEMOUT;
    tempActState.useDays = 0;
    tempActState.activeIndex = activeIndex;
    buyMode = BUYMODEMOUT;
    dayCount = 0;
    activeIndex = 0;

    WriteE2prom(actAddr, buf, sizeof(tempActState));
}


void ActivationCode::userBuyTheKey(QString strKey)
{
    printf("User Buy the Key !!! \n");
    QString actMode;

    usrCode = strKey;
    sn = getSN();


    actMode = QString("Life");
    actCode = getActivationCode(name + actMode, sn);
    printf("the Life Key is %s \n", actCode.toStdString().c_str());
    if(!usrCode.compare(usrCode, actCode))  //success to activate
    {
        printf("user buy the life mode \n");

        char *buf;
        ActivBuyState tempActState;
        buf = (char *)&tempActState;
        memset(buf, '\0', sizeof(tempActState));
        tempActState.buyMode = BUYMODELIFE;
        tempActState.useDays = 0;


        WriteE2prom(actAddr, buf, sizeof(tempActState));
        resultSta = ACTIVE_SUCCESS;
    }
    else
    {
        resultSta = ACTIVE_FAILED;
    }


    if(resultSta != ACTIVE_SUCCESS){

        actMode = QString("Year");
        for(int i = 1; i <= 10; i++){
            printf("\n>>>>>>>>>>>> Year Active index %d \n", i);
            int tempActiveI =  activeIndex + i;
            actCode = getActivationCode(name + actMode + QString::number(tempActiveI), sn);
            printf("the year key is %s activeIndex is %d \n", actCode.toStdString().c_str(), tempActiveI);

            if(!usrCode.compare(usrCode, actCode)){
                printf("user buy the year mode \n");
                char *buf;
                ActivBuyState tempActState;
                buf = (char *)&tempActState;
                memset(buf, '\0', sizeof(tempActState));

                tempActState.buyMode = BUYMODEYEAR;
                tempActState.useDays = 0;
                tempActState.activeIndex = tempActiveI;


//                for(int i = 0; i < sizeof(tempActState) ; i++){
//                    printf("0x%x ", buf[i]);
//                }
//                printf("Buffer bytes !! \n");

                WriteE2prom(actAddr, buf, sizeof(tempActState));

                resultSta = ACTIVE_SUCCESS;

                break;
            }
        }
    }


    if(resultSta == ACTIVE_FAILED){
        myMessageBox(trText_Active[9]);
    }else{
        emit close();
    }

}

void ActivationCode::userIntheBuy()
{
    if(isInTheBuy){
        printf("Confirm user has in the buy!! \n");
        resultSta = ACTIVE_SUCCESS;
        emit close();
    }
}

void ActivationCode::closeEvent(QCloseEvent *event)
{
    done(resultSta);
}
