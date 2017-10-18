#ifndef ACTIVTIMEWIDGET_H
#define ACTIVTIMEWIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include <QMessageBox>
#include <stdio.h>
#include <stdlib.h>
#include <QPainter>


#define GAIN_GRADE_SUM                  9
#define English_Active                         0
#define Chinese_Active                         1
#define French_Active                          2
#define Spanish_Active                         3
#define Portuguese_Active                      4
#define Russian_Active                         5
#define Italian_Active                         6
#define German_Active                          7
#define ChineseT_Active                        8       //Chinese Traditional  : CHT
#define Japanese_Active                        9       //Japanese: JPN
#define Korean_Active                          10      //Korean:   KOR
#define Czech_Active                           11      //捷克语

#define LANGUAGE_PATH_ACTIVE       			"/showlanguage/active/Language.tra"
#define LANGUAGE_ENG_PATH_ACTIVE   			"/showlanguage/active/LanguageEng.tra"
#define LANGUAGE_CHI_PATH_ACTIVE   			"/showlanguage/active/LanguageChi.tra"
#define LANGUAGE_FRE_PATH_ACTIVE   			"/showlanguage/active/LanguageFre.tra"
#define LANGUAGE_SPA_PATH_ACTIVE   			"/showlanguage/active/LanguageSpa.tra"
#define LANGUAGE_POR_PATH_ACTIVE   			"/showlanguage/active/LanguagePor.tra"
#define LANGUAGE_RUS_PATH_ACTIVE   			"/showlanguage/active/LanguageRus.tra"
#define LANGUAGE_ITA_PATH_ACTIVE   			"/showlanguage/active/LanguageIta.tra"
#define LANGUAGE_GER_PATH_ACTIVE               "/showlanguage/active/LanguageGer.tra"
#define LANGUAGE_CNT_PATH_ACTIVE               "/showlanguage/active/LanguageCnt.tra"
#define LANGUAGE_JPN_PATH_ACTIVE               "/showlanguage/active/LanguageJpn.tra"
#define LANGUAGE_KOR_PATH_ACTIVE               "/showlanguage/active/LanguageKor.tra"
#define LANGUAGE_CZE_PATH_ACTIVE               "/showlanguage/active/LanguageCze.tra"

#define QApplicationFont                QFont("WenQuanYi Zen Hei", 14 ,QFont::Normal)

extern char trText_Active[100][300];

int LoadTranslateText( QString dir, char text[][256]);
int SetCurrentTranslateText(int LanguageFlag, QString dirPath);

int myMessageBox(QString text);

#define BUYMODEYEAR 1
#define BUYMODELIFE 2
#define BUYMODEMOUT 3
enum ActivState{
    stateMouthIn = 0,
    stateMouthOut,
    stateYearIn,
    stateYearOut,
    stateAll,
};

#pragma pack (1)
typedef struct{
    char buyMode;
    int useDays;
    char activeIndex;
    char buf[19];
}ActivBuyState;
#pragma pack ()

void ReadE2prom(int addr , char *buf, int size);
void WriteE2prom(int addr , char *buf, int size);

namespace Ui {
class ActivTimeWidget;
}

class ActivTimeWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ActivTimeWidget(QWidget *parent = 0);
    ~ActivTimeWidget();

    void setActivState(ActivState state);
    void userAlreadyBuyIt(int mode);

public:
    void paintEvent(QPaintEvent *event);

signals:
    void signalUserCancel();
    void signalUserBuyTheKey(QString strKey);
    void signalUserTryIt();

private slots:
    void on_pushButton_Buy_clicked();
    void on_pushButton_Try_clicked();

    void on_pushButton_Cancel_clicked();

private:
    Ui::ActivTimeWidget *ui;

    bool isCancel;
};

#endif // ACTIVTIMEWIDGET_H
