#ifndef ACTIVATIONCODE_H
#define ACTIVATIONCODE_H

#include <QDialog>
#include <QFile>
#include <QCryptographicHash>
#include "activtimewidget.h"

#define ACTIVATED_FILE  "hasActived"

#define ACTIVE_SUCCESS  0
#define ACTIVE_FAILED   1
#define FILE_EXIST      2

#define ARM_CODE        1




namespace Ui {
class ActivationCode;
}

class ActivationCode : public QDialog
{
    Q_OBJECT
    
public:
    explicit ActivationCode(QString name, QString dirPath, QWidget *parent = 0);
    //ActivationCode(QString name, QString dirPath);
    ~ActivationCode();
    
private slots:
    void on_buttonBox_accepted();
    void on_buttonBox_rejected();

    QString getSN(void);
    QString getActivationCode(QString name, QString sn);
    QString rerangeCodeOrder(QString code);
    QString cutOffEqualChar(QString str);
    void createHasActivedFile(QString appPath);
    void dealFileExists();

public slots:
    void clearUserBuy();
    void userCancel();
    void userTryIt();
    void userBuyTheKey(QString strKey);
    void userIntheBuy();

protected:
    void closeEvent ( QCloseEvent * e );
    void paintEvent(QPaintEvent *);
    void hideEvent(QHideEvent *);

public:
    int currentModuleIndex();
    void getCurrentModuleTime();
    void checkActivTimeState();
    void useTimeActive(int language, QString path);




public:
    QString name;
    QString appPath;

private:
    Ui::ActivationCode *ui;

    ActivTimeWidget *timeActivWidget;

    QString sn;
    QString usrCode;
    QString actCode;

    int resultSta;

    int actAddr;
    int buyMode;
    int dayCount;
    int activeIndex;
    bool isInTheBuy;
};

#endif // ACTIVATIONCODE_H
