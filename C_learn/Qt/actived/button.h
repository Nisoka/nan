#ifndef BUTTON_H
#define BUTTON_H
#include "ourshare.h"


#define Big_Btn_Start_Y_Addr    36
#define Big_Btn_Start_X_Addr    689
#define Big_Btn_Dis             70

#define Mid_Btn_Start_Y_Addr    320
#define Mid_Btn_Start_X_Addr    645
#define Mid_Btn_Dis_X           3
#define Mid_Btn_Dis_Y           45
#define Mid_Btn_W               75
#define Mid_Btn_H               60

#define Little_Icon_Start_Y_Addr CurveQFrame_Y

#define Little_Icon_Start_X_Addr 646
#define Little_Icon_W 35

extern QRect LossButtonRect;
extern QRect SlopeButtonRect;


extern QRect HelpButtonRect;
extern QRect HideButtonRect;
extern QRect CloseButtonRect;
extern QRect GainButtonRect;
extern QRect TestButtonRect;//测量按钮的位置
extern QRect FIPButtonRect; //FIP
extern QRect AutoTestButtonRect;//自动测量按钮的位置
extern QRect RealTimeTestButtonRect;//实时测量按钮的位置

extern QRect SetButtonRect;//设置按钮的位置
extern QRect FileButtonRect;//文件按钮的位置
extern QRect CtrlButtonRect;//操作按钮的位置
extern QRect ResultButtonRect;//结果按钮的位置

extern QRect ShortcutsButtonRect;//快捷键按钮的位置


extern QRect CursorButtonRect;//光标按钮的位置
extern QRect TraceButtonRect;//迹线按钮的位置
extern QRect EventButtonRect;//事件按钮的位置
extern QRect TableButtonRect;//表按钮的位置

extern QRect FileOpenButtonRect;//文件打开按钮的位置
extern QRect FileSaveButtonRect;//文件保存按钮的位置
extern QRect FileSaveSetButtonRect;//文件保存设置按钮的位置
extern QRect DoubleTestButtonRect;
extern QRect TraceNextButtonRect;
extern QRect TraceRefDelButtonRect;
extern QRect TraceRefButtonRect;
extern QRect DiffButtonRect;

extern QRect MoveLeftButtonRect;//左移按钮的位置
extern QRect MoveRightButtonRect;//右移按钮的位置
extern QRect CursorLButtonRect;//光标L按钮的位置
extern QRect CursorRButtonRect;//光标R按钮的位置
//extern QRect CursorLRButtonRect;// 光标LR按钮的位置
extern QRect CursorAButtonRect;// 光标A按钮的位置
extern QRect CursorBButtonRect;// 光标B按钮的位置

extern QRect SomeEventListButtonRect;//4行列表钮的位置
extern QRect FullEventListButtonRect;//全屏列表按钮的位置
extern QRect TableTraceButtonRect;//返回迹线按钮

extern QRect nm1310TraceButtonRect;//1310迹线按钮位置
extern QRect nm1550TraceButtonRect;//1550迹线按钮位置
extern QRect nm1490TraceButtonRect;//1550迹线按钮位置
extern QRect nm1650TraceButtonRect;//1550迹线按钮位置
extern QRect nm1625TraceButtonRect;//1625迹线按钮位置
extern QRect nm850TraceButtonRect;//1550迹线按钮位置
extern QRect nm1300TraceButtonRect;//1550迹线按钮位置
extern QRect fileInfoButtonRect;
extern QRect offlineAnalysisButtonRect;
extern QRect TraceButtonArray[WAVELENGTH_SUM];

extern QRect AddEventButtonRect;//添加事件按钮位置
extern QRect DeleteEventButtonRect;//删除事件按钮位置
extern QRect EventSectionButtonRect;//事件跨段按钮位置


extern QRect AutoZoomUpButtonRect;//自动放大按钮位置
extern QRect VZoomUpButtonRect;//垂直放大按钮位置
extern QRect VZoomDownButtonRect;//水放大按钮位置
extern QRect ZoomUpButtonRect;
extern QRect ZoomDownButtonRect;
extern QRect ZoomFullButtonRect;
extern QRect MoveUDButtonRect;
extern QRect MoveLRButtonRect;
extern QRect HandButtonRect;
extern QRect RubberBandButtonRect;
extern QRect MaintainButtonRect;
extern QRect DebugButtonRect;


extern QColor TestTextUpColor;//未按下的颜色
extern QColor TestTextDownColor;//按下的颜色

extern QColor FunTextUpColor;//未按下的颜色
extern QColor FunTextDownColor;//按下的颜色

extern QString TestButtonUpMap;
extern QString TestButtonDownMap;
extern QString FIPButtonDownMap;
extern QString FIPButtonUpMap;
extern QString FunSmallButtonUpMap;
extern QString FunSmallButtonDownMap;

extern QString FunButtonUpMap;
extern QString FunButtonDownMap;
extern QString FunLittleButtonDownMap;
extern QString FunLittleButtonUpMap;

extern QString doubleTestButtonUpMap;
extern QString doubleTestButtonDownMap;

extern QString bidiTraceEventsButtonUpMap;
extern QString bidiTraceEventsButtonDownMap;

extern QString NextTraceButtonUpMap;
extern QString NextTraceButtonDownMap;

extern QString RefTraceDelButtonUpMap;
extern QString RefTraceDelButtonDownMap;

extern QString DiffButtonUpMap;
extern QString DiffButtonDownMap;

extern QString RefTraceButtonUpMap;
extern QString RefTraceButtonDownMap;

extern QString FileOpenButtonUpMap;
extern QString FileOpenButtonDownMap;

extern QString FileSaveButtonUpMap;
extern QString FileSaveButtonDownMap;

extern QString FileSaveSetButtonUpMap;
extern QString FileSaveSetButtonDownMap;

extern QString MoveLeftButtonUpMap;
extern QString MoveLeftButtonDownMap;

extern QString MoveRightButtonUpMap;
extern QString MoveRightButtonDownMap;

extern QString CursorLButtonUpMap;
extern QString CursorLButtonDownMap;

extern QString CursorLButtonUpMap;
extern QString CursorLButtonDownMap;

extern QString CursorRButtonUpMap;
extern QString CursorRButtonDownMap;

extern QString CursorLRButtonUpMap;
extern QString CursorLRButtonDownMap;

extern QString CursorAButtonUpMap;
extern QString CursorAButtonDownMap;
extern QString CursorABigButtonUpMap;
extern QString CursorABigButtonDownMap;

extern QString CursorBButtonUpMap;
extern QString CursorBButtonDownMap;
extern QString CursorBBigButtonUpMap;
extern QString CursorBBigButtonDownMap;

extern QString ZoomUpButtonUpMap;
extern QString ZoomUpButtonDownMap;

extern QString ZoomDownButtonUpMap;
extern QString ZoomDownButtonDownMap;

extern QString VZoomUpButtonUpMap;
extern QString VZoomUpButtonDownMap;

extern QString VZoomDownButtonUpMap;
extern QString VZoomDownButtonDownMap;

extern QString ZoomFullButtonUpMap;
extern QString ZoomFullButtonDownMap;

extern QString MoveLRButtonUpMap;
extern QString MoveLRButtonDownMap;

extern QString MoveUDButtonUpMap;
extern QString MoveUDButtonDownMap;

extern QString AddEventButtonUpMap;
extern QString AddEventButtonDownMap;

extern QString DeleteEventButtonUpMap;
extern QString DeleteEventButtonDownMap;

extern QString EventSectionButtonUpMap;
extern QString EventSectionButtonDownMap;

extern QString HandButtonUpMap;
extern QString HandButtonDownMap;

extern QString RubberBandButtonUpMap;
extern QString RubberBandButtonDownMap;
 
extern QString InfoButtonUpMap;
extern QString InfoButtonDownMap;

extern QString ItemButtonUpMap;
extern QString ItemButtonDownMap;
//维护排障界面
extern QString MaintainWinStartBtnUpMap;
extern QString MaintainWinStartBtnDownMap;

extern char *StopStr;//"Stop";
extern char *StartStr;//"Start",
extern char *SetStr;//"Setup";
extern char *FileStr;//"Save";
extern char *CtrlStr;//"Operate";
extern char *CursorStr;//"Cursor";
extern char *TraceStr;//"Trace";
extern char *EventStr;//"Event";
extern char *TableStr;//"Table";
extern char *ResultStr;//"Result";

class  PushBtn: public QPushButton
{
    Q_OBJECT
public:
    PushBtn(QWidget *parent);
   const  char *UpText;
   const  char *DownText;
   const char *Font_Set;
    char TextSize;
    char UpDown_Flag;
    QString UpMap;
    QString DownMap;

    QColor UpTextColor;
    QColor DownTextColor;

    int TextOffset;
    int Bold_Normal;
    int ID;
    void Init(QRect Rect);
    bool Check_State;
    bool isHide;
    // void (*FunP)(QWidget *parent);

   // char Time_Press_Enable;
   // QTimer Time_Press_interval;

protected:
    void mousePressEvent(QMouseEvent *) ;
    void mouseReleaseEvent(QMouseEvent *) ;
    //void keyPressEvent( QKeyEvent *k );
    void paintEvent(QPaintEvent *);
private:

signals:
    void MYmousePress(void);
    void MYmouseRelease(void);
    void Send_ID(int);
    void Time_interval_out(void);
public slots:
     void ShowDownButton(void);
     void ShowUpButton(void);
     void HideButton(void);
     void SendMYmousePress(void);
     void SendMYmouseRelease(void);
     void Send_Send_ID(int index);
     void ChangeStatus(void);
};




class  FunButton: public QPushButton
{
    Q_OBJECT
public:
    char *UpText;
    char *DownText;
    QString UpMap;
    QString DownMap;
    char TextSize;
    QColor UpTextColor;
    QColor DownTextColor;
    FunButton(QWidget *parent);

    void Init(QRect Rect);
    void ChangeTextAndMap(char *text,QColor TestColor,QString mapstr);
protected:
    void mousePressEvent(QMouseEvent *) ;
    void mouseReleaseEvent(QMouseEvent *) ;
private:

signals:
    void MYmousePress(void);
    void MYmouseRelease(void);
    void PressTimeout(void);
public slots:
    void ShowUpButton(void);
    void ShowDownButton(void);
    void SendMYmousePress(void);
};


class  InfoButton: public QPushButton
{
    Q_OBJECT
public:
    char *UpText;
    char *DownText;
    QString UpMap;
    QString DownMap;
    char TextSize;
    int ID;
    char InitUpDownFlag;
    QColor UpTextColor;
    QColor DownTextColor;
    InfoButton(QWidget *parent);
    void Init(QRect Rect);
    void ChangeTextAndMap(char *text,QColor TestColor,QString mapstr);
protected:
    void mousePressEvent(QMouseEvent *) ;
    void mouseReleaseEvent(QMouseEvent *) ;
    // void enterEvent(QEvent * event) ;
    // void leaveEvent ( QEvent * event ) ;
    // void paintEvent(QPaintEvent *event) ;
private:

signals:
    void MYmousePress(int);
    void MYmouseRelease(void);
    void MYmouseDown(void);
    void HideChildBtn(void);
public slots:
    void ShowUpButton(int);
    void HideButton(void);
    void SendMYmousePress(void);

};

#endif // BUTTON_H
