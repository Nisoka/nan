#include "ourshare.h"
#include "button.h"
#include "measure.h"

#define MapDirSize 1000
#define ButtonTextSize 50

U8 FirstButtonID=CursorBtn;//一级按钮id号



QRect LossButtonRect(240,2,60,22);
QRect SlopeButtonRect(240,26,60,22);


QRect HelpButtonRect(660,410,32,32);
QRect HideButtonRect(705,410,32,32);
QRect CloseButtonRect(750,410,32,32);

QRect GainButtonRect(640,3,50,50);//启动增益界面按钮

QRect ShortcutsButtonRect(600,50,40,22);    //快捷键按钮的位置

QRect TestButtonRect(Big_Btn_Start_X_Addr,Big_Btn_Start_Y_Addr-30,100,80);//测量按钮的位置
QRect FIPButtonRect(Little_Icon_Start_X_Addr,TestButtonRect.top()+10,Little_Icon_W,Little_Icon_W);//FIP按钮
QRect AutoTestButtonRect(Big_Btn_Start_X_Addr+3,Big_Btn_Start_Y_Addr+Big_Btn_Dis-45,45,36);//自动测量的位置
QRect RealTimeTestButtonRect(AutoTestButtonRect.right() + 7, AutoTestButtonRect.top(), 45, 36);//实时测量按钮的位置

QRect MaintainButtonRect(TestButtonRect.left(),TestButtonRect.top(),TestButtonRect.width(),TestButtonRect.height());//维护按钮
QRect DebugButtonRect(MaintainButtonRect.left(), MaintainButtonRect.bottom()-10, MaintainButtonRect.width(), MaintainButtonRect.height());//排障按钮

#ifdef ENABLE_WORKER
QRect SetButtonRect(Big_Btn_Start_X_Addr,DebugButtonRect.bottom()-10,100,80);//设置按钮的位置
QRect FileButtonRect(Big_Btn_Start_X_Addr,SetButtonRect.bottom()-10,100,80);//文件按钮的位置
QRect CtrlButtonRect(Big_Btn_Start_X_Addr,FileButtonRect.bottom()-10,100,80);//操作按钮的位置
#else
QRect SetButtonRect(Big_Btn_Start_X_Addr,Big_Btn_Start_Y_Addr+Big_Btn_Dis,100,80);//设置按钮的位置
QRect FileButtonRect(Big_Btn_Start_X_Addr,Big_Btn_Start_Y_Addr+Big_Btn_Dis*2,100,80);//文件按钮的位置
QRect CtrlButtonRect(Big_Btn_Start_X_Addr,Big_Btn_Start_Y_Addr+Big_Btn_Dis*3,100,80);//操作按钮的位置
//QRect ResultButtonRect(Big_Btn_Start_X_Addr,110,100,80);//结果按钮的位置
#endif

#ifdef ENABLE_WORKER
QRect CursorButtonRect(Mid_Btn_Start_X_Addr,Mid_Btn_Start_Y_Addr+30,Mid_Btn_W,Mid_Btn_H);//光标按钮的位置
QRect TableButtonRect(Mid_Btn_Start_X_Addr+Mid_Btn_W+Mid_Btn_Dis_X, CursorButtonRect.top(), Mid_Btn_W,Mid_Btn_H);//表按钮的位置
#else
QRect CursorButtonRect(Mid_Btn_Start_X_Addr,Mid_Btn_Start_Y_Addr,Mid_Btn_W,Mid_Btn_H);//光标按钮的位置
QRect TableButtonRect(Mid_Btn_Start_X_Addr+Mid_Btn_W+Mid_Btn_Dis_X,Mid_Btn_Start_Y_Addr+Mid_Btn_Dis_Y,Mid_Btn_W,Mid_Btn_H);//表按钮的位置
#endif
QRect TraceButtonRect(Mid_Btn_Start_X_Addr+Mid_Btn_W+Mid_Btn_Dis_X,Mid_Btn_Start_Y_Addr,Mid_Btn_W,Mid_Btn_H);//迹线按钮的位置
QRect EventButtonRect(Mid_Btn_Start_X_Addr,Mid_Btn_Start_Y_Addr+Mid_Btn_Dis_Y,Mid_Btn_W,Mid_Btn_H);//事件按钮的位置

QRect FileOpenButtonRect(Little_Icon_Start_X_Addr,50,Little_Icon_W,Little_Icon_W);//文件打开按钮的位置
QRect FileSaveButtonRect(Little_Icon_Start_X_Addr,50+35,Little_Icon_W,Little_Icon_W);//文件保存按钮的位置
QRect FileSaveSetButtonRect(Little_Icon_Start_X_Addr,50+35*2,Little_Icon_W,Little_Icon_W);//文件保存设置按钮的位置

QRect DoubleTestButtonRect(Little_Icon_Start_X_Addr,60+35*3,Little_Icon_W,Little_Icon_W);//双向测试功能按钮
QRect TraceRefButtonRect(Little_Icon_Start_X_Addr,60+35*3,Little_Icon_W,Little_Icon_W);//设置当前为参考曲线按钮位置
QRect TraceNextButtonRect(Little_Icon_Start_X_Addr,60+35*4,Little_Icon_W,Little_Icon_W);//下一条曲线按钮位置
QRect TraceRefDelButtonRect(Little_Icon_Start_X_Addr,60+35*5,Little_Icon_W,Little_Icon_W);//下一条曲线按钮位置
QRect DiffButtonRect(Little_Icon_Start_X_Addr, 60+35*6, Little_Icon_W, Little_Icon_W);//曲线差异按钮位置

QRect MoveLeftButtonRect(Little_Icon_Start_X_Addr,110,Little_Icon_W,Little_Icon_W);//左移按钮的位置
QRect MoveRightButtonRect(Little_Icon_Start_X_Addr,142,Little_Icon_W,Little_Icon_W);//右移按钮的位置
QRect CursorLButtonRect(Little_Icon_Start_X_Addr,171,Little_Icon_W,Little_Icon_W);//光标L按钮的位置
QRect CursorRButtonRect(Little_Icon_Start_X_Addr,203,Little_Icon_W,Little_Icon_W);//光标R按钮的位置
//QRect CursorLRButtonRect(Little_Icon_Start_X_Addr,245,Little_Icon_W,Little_Icon_W);// 光标LR按钮的位置
QRect CursorAButtonRect(Little_Icon_Start_X_Addr,235,Little_Icon_W,Little_Icon_W);// 光标A按钮的位置
QRect CursorBButtonRect(Little_Icon_Start_X_Addr,267,Little_Icon_W,Little_Icon_W);// 光标B按钮的位置

QRect AddEventButtonRect(Little_Icon_Start_X_Addr,181,Little_Icon_W,Little_Icon_W);//添加事件按钮位置
QRect DeleteEventButtonRect(Little_Icon_Start_X_Addr,181+35,Little_Icon_W,Little_Icon_W);//删除事件按钮位置
QRect EventSectionButtonRect(Little_Icon_Start_X_Addr,181+35*2,Little_Icon_W,Little_Icon_W);//事件跨段按钮位置
QRect AutoZoomUpButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr-Little_Icon_W,Little_Icon_W,Little_Icon_W);//自动放大按钮位置
QRect VZoomUpButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr,Little_Icon_W,Little_Icon_W);//垂直放大按钮位置
QRect VZoomDownButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+Little_Icon_W,Little_Icon_W,Little_Icon_W);//水放大按钮位置
QRect ZoomUpButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+2*Little_Icon_W,Little_Icon_W,Little_Icon_W);
QRect ZoomDownButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+3*Little_Icon_W,Little_Icon_W,Little_Icon_W);
QRect ZoomFullButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+4*Little_Icon_W,Little_Icon_W,Little_Icon_W);
QRect MoveUDButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+5*Little_Icon_W,Little_Icon_W,Little_Icon_W);
QRect MoveLRButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+6*Little_Icon_W,Little_Icon_W,Little_Icon_W);
QRect HandButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+5*Little_Icon_W,Little_Icon_W,Little_Icon_W);
QRect RubberBandButtonRect(Little_Icon_Start_X_Addr,Little_Icon_Start_Y_Addr+6*Little_Icon_W,Little_Icon_W,Little_Icon_W);


QRect SomeEventListButtonRect(Little_Icon_Start_X_Addr,181,Little_Icon_W,Little_Icon_W);//4行列表钮的位置
QRect FullEventListButtonRect(Little_Icon_Start_X_Addr,213,Little_Icon_W,Little_Icon_W);//全屏列表按钮的位置
QRect TableTraceButtonRect(Little_Icon_Start_X_Addr,245,Little_Icon_W,Little_Icon_W);//返回迹线按钮

QRect nm1310TraceButtonRect(Little_Icon_Start_X_Addr,65,Little_Icon_W,Little_Icon_W);//1310迹线按钮位置
QRect nm1550TraceButtonRect(Little_Icon_Start_X_Addr,65+35,Little_Icon_W,Little_Icon_W);//1550迹线按钮位置
QRect nm1490TraceButtonRect(Little_Icon_Start_X_Addr,65+35*2,Little_Icon_W,Little_Icon_W);//1550迹线按钮位置
QRect nm1650TraceButtonRect(Little_Icon_Start_X_Addr,65+35*3,Little_Icon_W,Little_Icon_W);//1550迹线按钮位置
QRect nm1625TraceButtonRect(Little_Icon_Start_X_Addr,65+35*4,Little_Icon_W,Little_Icon_W);//1625迹线按钮位置
QRect nm850TraceButtonRect(Little_Icon_Start_X_Addr,65+35*5,Little_Icon_W,Little_Icon_W);//1550迹线按钮位置
QRect nm1300TraceButtonRect(Little_Icon_Start_X_Addr,65+35*6,Little_Icon_W,Little_Icon_W);//1550迹线按钮位置
QRect fileInfoButtonRect(Little_Icon_Start_X_Addr,65+35*7,Little_Icon_W,Little_Icon_W);//file info按钮位置
QRect offlineAnalysisButtonRect(Little_Icon_Start_X_Addr,65+35*8,Little_Icon_W,Little_Icon_W);//file info按钮位置

QRect TraceButtonArray[WAVELENGTH_SUM] = {nm1310TraceButtonRect, nm1550TraceButtonRect, nm1490TraceButtonRect,
                                         nm1650TraceButtonRect, nm1625TraceButtonRect, nm850TraceButtonRect, nm1300TraceButtonRect};


QColor TestTextUpColor(255,255,255);//未按下的颜色
QColor TestTextDownColor(255,255,255);//按下的颜色


QColor FunTextUpColor(255,255,255);//未按下的颜色
QColor FunTextDownColor(0,0,0);//按下的颜色

QString TestButtonUpMap="TestButtonUp.png";
QString TestButtonDownMap="TestButtonDown.png";
QString FIPButtonDownMap="FIPButtonDown.png";
QString FIPButtonUpMap="FIPButtonUp.png";
QString FunSmallButtonUpMap="FunSmallButtonUp.png";
QString FunSmallButtonDownMap="FunSmallButtonDown.png";

QString FunButtonUpMap="FunButtonUp.png";
QString FunButtonDownMap="FunButtonDown.png";
QString FunLittleButtonDownMap="FunLittleButtonDown.png";
QString FunLittleButtonUpMap="FunLittleButtonUp.png";

QString NextTraceButtonUpMap="nextTraceButtonUp";
QString NextTraceButtonDownMap="nextTraceButtonDown";

QString doubleTestButtonUpMap="doubleTestButtonUp";
QString doubleTestButtonDownMap="doubleTestButtonDown";

QString bidiTraceEventsButtonUpMap="bidiTraceEventsButtonUp";
QString bidiTraceEventsButtonDownMap="bidiTraceEventsButtonDown";

QString RefTraceDelButtonUpMap="refDisTraceButtonUp.png";
QString RefTraceDelButtonDownMap="refDisTraceButtonDown.png";

QString DiffButtonUpMap="diffButtonUp.png";
QString DiffButtonDownMap="diffButtonDown.png";

QString RefTraceButtonUpMap="refTraceButtonUp";
QString RefTraceButtonDownMap="refTraceButtonDown";

QString FileOpenButtonUpMap="FileOpenButtonUp.png";
QString FileOpenButtonDownMap="FileOpenButtonDown.png";

QString FileSaveButtonUpMap="FileSaveButtonUp.png";
QString FileSaveButtonDownMap="FileSaveButtonDown.png";

QString FileSaveSetButtonUpMap="FileSaveSetButtonUp.png";
QString FileSaveSetButtonDownMap="FileSaveSetButtonDown.png";

QString CursorLButtonUpMap="CursorLButtonUp.png";
QString CursorLButtonDownMap="CursorLButtonDown.png";

QString CursorRButtonUpMap="CursorRButtonUp.png";
QString CursorRButtonDownMap="CursorRButtonDown.png";

QString CursorLRButtonUpMap="CursorLRButtonUp.png";
QString CursorLRButtonDownMap="CursorLRButtonDown.png";

QString CursorAButtonUpMap="CursorAButtonUp.png";
QString CursorAButtonDownMap="CursorAButtonDown.png";
QString CursorABigButtonUpMap="CursorA2ButtonUp.png";
QString CursorABigButtonDownMap="CursorA2ButtonDown.png";

QString CursorBButtonUpMap="CursorBButtonUp.png";
QString CursorBButtonDownMap="CursorBButtonDown.png";
QString CursorBBigButtonUpMap="CursorB2ButtonUp.png";
QString CursorBBigButtonDownMap="CursorB2ButtonDown.png";

QString ZoomUpButtonUpMap="ZoomUpButtonUp.png";
QString ZoomUpButtonDownMap="ZoomUpButtonDown.png";

QString ZoomDownButtonUpMap="ZoomDownButtonUp.png";
QString ZoomDownButtonDownMap="ZoomDownButtonDown.png";

QString VZoomUpButtonUpMap="VZoomUpButtonUp.png";
QString VZoomUpButtonDownMap="VZoomUpButtonDown.png";

QString VZoomDownButtonUpMap="VZoomDownButtonUp.png";
QString VZoomDownButtonDownMap="VZoomDownButtonDown.png";

QString ZoomFullButtonUpMap="ZoomFullButtonUp.png";
QString ZoomFullButtonDownMap="ZoomFullButtonDown.png";

QString MoveUDButtonUpMap="MoveUDButtonUp.png";
QString MoveUDButtonDownMap="MoveUDButtonDown.png";

QString MoveLRButtonUpMap="MoveLRButtonUp.png";
QString MoveLRButtonDownMap="MoveLRButtonDown.png";

QString HandButtonUpMap="HandButtonUp.png";
QString HandButtonDownMap="HandButtonDown.png";


QString MoveLeftButtonUpMap="MoveLeftButtonUp.png";
QString MoveLeftButtonDownMap="MoveLeftButtonDown.png";

QString MoveRightButtonUpMap="MoveRightButtonUp.png";
QString MoveRightButtonDownMap="MoveRightButtonDown.png";

QString AddEventButtonUpMap="AddEventButtonUp.png";
QString AddEventButtonDownMap="AddEventButtonDown.png";
QString EventSectionButtonUpMap="EventSectionLaunchUp.png";
QString EventSectionButtonDownMap="EventSectionLaunchDown.png";

QString DeleteEventButtonUpMap="DeleteEventButtonUp.png";
QString DeleteEventButtonDownMap="DeleteEventButtonDown.png";

QString RubberBandButtonUpMap="RubberBandButtonUp.png";
QString RubberBandButtonDownMap="RubberBandButtonDown.png";


QString InfoButtonUpMap="InfoButtonUp.png";
QString InfoButtonDownMap="InfoButtonDown.png";

QString ItemButtonUpMap="ItemButtonUp.png";
QString ItemButtonDownMap="ItemButtonDown.png";

//维护排障界面
QString MaintainWinStartBtnUpMap="maintainBtnBk_Up.png";
QString MaintainWinStartBtnDownMap="maintainBtnBk_Down.png";


char *StopStr=trText[93];//"Stop";
char *StartStr=trText[13];//"Start",
char *SetStr=trText[14];//"Setup";
char *FileStr=trText[15];//"File";
char *CtrlStr=trText[16];//"Operate";
char *CursorStr=trText[0];//"Cursor";
char *TraceStr=trText[12];//"Trace";
char *EventStr=trText[1];//"Event";
char *TableStr=trText[11];//"Table";
char *ResultStr=trText[98];//"Result";




InfoButton::InfoButton(QWidget *parent):QPushButton(parent)
{
    //设置按钮背景透明
    QPalette palette;
    palette.setBrush(QPalette::Button,QBrush(QColor(255,255,255,0)));// 0 是完全透明
    setPalette(palette);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::NoFocus);//去掉按钮虚线，就是失去焦点
   // TextSize=12;
   // UpText=CtrlStr;
   // DownText=CtrlStr;
    UpMap=MAX711_PATH+Interface_Type_PATH+InfoButtonUpMap;
    DownMap=MAX711_PATH+Interface_Type_PATH+InfoButtonDownMap;
    UpTextColor=FunTextUpColor;
    DownTextColor=FunTextDownColor;

    //Init(LossButtonRect);
    this->show();
}
void InfoButton::Init(QRect Rect)
{
    setGeometry(Rect);
    if(InitUpDownFlag)
        ChangeTextAndMap(DownText,DownTextColor,DownMap);
    else
        ChangeTextAndMap(UpText,UpTextColor,UpMap);
}
void InfoButton::ChangeTextAndMap(char *text,QColor TextColor,QString mapstr)
{

    QPixmap mypixmap;
    mypixmap.load(mapstr);

    QPainter painte(&mypixmap);
    painte.setPen(TextColor);
    if(System_para.LanguageFlag == Russian || System_para.LanguageFlag == Czech)
        //painte.setFont(QFont("Arial",TextSize,QFont::Normal));
        painte.setFont(InfoButtonFontRussian(TextSize));
    else
        //painte.setFont(QFont("WenQuanYi Zen Hei",TextSize,QFont::Normal));
        painte.setFont(InfoButtonFont(TextSize));

    painte.drawText(0,0,size().width(),size().height(), Qt::AlignCenter, tr(text));//在rect方框中间显现

    setIconSize(this->size());//按照按钮的尺寸装载
    setIcon(QIcon(mypixmap));//装载到按钮背景
    //setIconSize(mypixmap.size());//按照mypixmap的尺寸装载
    setFlat(true);
   // qDebug()<<tr("width")<<size().width()<<tr("height")<<size().height();

}
void InfoButton::mousePressEvent(QMouseEvent *)
{
   // FirstButtonID=CtrlBtn;
    ChangeTextAndMap(DownText,DownTextColor,DownMap);
    emit MYmousePress(ID);
    emit MYmouseDown();

}
void InfoButton::mouseReleaseEvent(QMouseEvent *)
{
    //ChangeTextAndMap(UpText,UpTextColor,UpMap);
}
void InfoButton::ShowUpButton(int)
{  
   ChangeTextAndMap(UpText,UpTextColor,UpMap);
   this->show();
}
void InfoButton::SendMYmousePress(void)
{
    ChangeTextAndMap(DownText,DownTextColor,DownMap);
    emit MYmousePress(ID);
    emit MYmouseDown();
}

void InfoButton::HideButton(void)
{
   this->hide();
}



FunButton::FunButton(QWidget *parent):QPushButton(parent)
{
    //设置按钮背景透明
    QPalette palette;
    palette.setBrush(QPalette::Button,QBrush(QColor(255,255,255,0)));// 0 是完全透明
    setPalette(palette);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::NoFocus);//去掉按钮虚线，就是失去焦点
    TextSize=10;
    //UpText=" ";
   // DownText=" ";

    UpTextColor=FunTextUpColor;
    DownTextColor=FunTextDownColor;

    this->show();

}
void FunButton::SendMYmousePress(void)
{
    emit MYmousePress();
}

void FunButton::Init(QRect Rect)
{
    setGeometry(Rect);
    ChangeTextAndMap(UpText,UpTextColor,UpMap);
}
void FunButton::ChangeTextAndMap(char *text,QColor TextColor,QString mapstr)
{

    QPixmap mypixmap;
    mypixmap.load(mapstr);

    QPainter painte(&mypixmap);
    painte.setPen(TextColor);
   // painte.setFont(QFont("wenquanyi",10,QFont::Normal));//QFont::Bold QFont::Normal
    //painte.drawText(0,0,size().width(),size().height(), Qt::AlignCenter, tr(text));//在rect方框中间显现

    setIconSize(this->size());//按照按钮的尺寸装载
    setIcon(QIcon(mypixmap));//装载到按钮背景
    //setIconSize(mypixmap.size());//按照mypixmap的尺寸装载
    setFlat(true);

}
void FunButton::mousePressEvent(QMouseEvent *)
{
    ChangeTextAndMap(DownText,DownTextColor,DownMap);
    if(gb_start_flag==0)
    emit MYmousePress();
}
void FunButton::mouseReleaseEvent(QMouseEvent *)
{
   // ChangeTextAndMap(UpText,UpTextColor,UpMap);
    //emit MYmouseRelease();
}
void FunButton::ShowUpButton(void)
{
    ChangeTextAndMap(UpText,UpTextColor,UpMap);
    this->show();
}
void FunButton::ShowDownButton(void)
{
   ChangeTextAndMap(UpText,UpTextColor,DownMap);
   this->show();
}



PushBtn::PushBtn(QWidget *parent):QPushButton(parent)
{
    //设置按钮背景透明
    QPalette palette;
    palette.setBrush(QPalette::Button,QBrush(QColor(255,255,255,0)));// 0 是完全透明
    setPalette(palette);
    setAutoFillBackground(true);
    setFocusPolicy(Qt::NoFocus);//去掉按钮虚线，就是失去焦点

    if(System_para.LanguageFlag == Italian)
        TextSize=FONT_SIZE_18;
    else
        TextSize=FONT_SIZE_20;

    UpMap=MAX711_PATH+Interface_Type_PATH+FunButtonUpMap;
    DownMap=MAX711_PATH+Interface_Type_PATH+FunButtonDownMap;
    UpTextColor=FunTextUpColor;
    DownTextColor=FunTextDownColor;
    UpDown_Flag=0;

    Font_Set="WenQuanYi Zen Hei";
    TextOffset =15;
    Bold_Normal=QFont::Normal;
    Check_State=FALSE;// true 是点击按下松开抬起；false 是点击按下，再点击抬起；
    isHide = false;
}


void PushBtn::mousePressEvent(QMouseEvent* )
{
    if(isHide)
        return;

    if(Check_State)
    {
        UpDown_Flag=1;
        this->repaint();
        emit MYmousePress();
        emit Send_ID(ID);
    }
    else
    {
        emit MYmousePress();
        emit Send_ID(ID);
    }
}

void PushBtn::mouseReleaseEvent(QMouseEvent* )
{
    if(isHide)
        return;

    emit MYmouseRelease();
    if(Check_State)
    {
        UpDown_Flag=0;
        this->repaint();
    }
}
void PushBtn::SendMYmousePress(void)
{
    if(isHide)
        return;

    emit MYmousePress();

}
void PushBtn::SendMYmouseRelease(void)
{
    if(isHide)
        return;

    emit MYmouseRelease();
}

void PushBtn::Send_Send_ID(int index )
{
    if(isHide)
        return;

     emit Send_ID(index);
}
void PushBtn::ChangeStatus(void)
{
    if(isHide)
        return ;

    if(this->UpDown_Flag == 1)
    {
        this->ShowUpButton();
    }
    else
    {
        this->ShowDownButton();
    }
}
void PushBtn::ShowDownButton(void)
{
    if(isHide)
        return ;

    UpDown_Flag=1;
    this->show();
    this->repaint();
}
void PushBtn::ShowUpButton(void)
{
    if(isHide)
        return ;

    UpDown_Flag=0;
    this->show();
    this->repaint();
}
void PushBtn::HideButton(void)
{
    if(isHide)
        return;

    this->hide();
}
void PushBtn::paintEvent(QPaintEvent *)
{
    if(isHide)
        return ;

    QPixmap  BtnMap;
    QPainter painte(this);

    painte.setFont(QFont(Font_Set,TextSize,Bold_Normal));

    if(UpDown_Flag)
    {
        BtnMap.load(DownMap);
        painte.setPen(DownTextColor);
        painte.drawPixmap(0,0,size().width(),size().height(),BtnMap);
        painte.drawText(0,TextOffset,size().width(),size().height(), Qt::AlignHCenter, tr(DownText));//在rect方框中间显现
    }
    else
    {
        BtnMap.load(UpMap);
        painte.setPen(UpTextColor);
        painte.drawPixmap(0,0,size().width(),size().height(),BtnMap);
        painte.drawText(0,TextOffset,size().width(),size().height(), Qt::AlignHCenter, tr(UpText));//在rect方框中间显现
    }

}







