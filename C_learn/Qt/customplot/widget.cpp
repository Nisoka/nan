#include "widget.h"
#include "ui_widget.h"

#define BAR_COUNT 1


#define PLOT_X  65
#define PLOT_Y  0
#define PLOT_W  T_W - PLOT_X
#define PLOT_H  T_H
#define PLOT_LINE_CNT 6

#define YAXIS_LEN 120
#define XAXIS_LEN 4

#define POWER_RANGE 120

#define MARGIN_UP 12
#define PLOT_MARGIN 15


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    initData();
    initUi();
    QPalette pal;
    pal.setColor(QPalette::Background, Qt::black);
    this->setPalette(pal);
//    testInit();

}

Widget::~Widget()
{
    delete ui;
}

void Widget::UpdateLimit()
{
    QString strTemp;
    UpdateCurrentUnit();
    if(currentMode == ANALOG_TV){
        setLimit(limitTV_show);
    }else{
        setLimit(limitDVB_show);
    }
    strTemp.sprintf("V/A: %.2f ~ %.2f dB", limitVA_show[1], limitVA_show[0]);
    labelVALimit->setText(strTemp);
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);       //没什么作用么？

    QString labelText;
    QPainter *painter = new QPainter(this);
//    QBrush brush;
//    brush.setColor(Qt::black);
//    painter->setBrush(brush);
//    painter->drawRect(0, 0, 800, 600);

    QPen pen;
    pen.setColor(Qt::white);
    painter->setPen(pen);

    for(int i = 0; i < PLOT_LINE_CNT+1; i++){
        painter->drawText(QRect(PLOT_X-40, PLOT_Y+i*(PLOT_H/(PLOT_LINE_CNT+0.5)), 40, 20),
                          Qt::AlignRight, labelText.sprintf("%d", int(upLine - i*20)));

    }

    QString tempUnit = "(dBuV)";
    switch(g_dbmUnit){
    case unitLimit_dBuv:
        tempUnit = "(dBuV)";
        break;
    case unitLimit_dBmv:
        tempUnit = "(dBmV)";
        break;
    case unitLimit_dBm:
        tempUnit = "(dBm)";
        break;
    }

    painter->rotate(90);
    QFont fnt;
    fnt.setPixelSize(20);
    painter->setFont(fnt);
    painter->drawText(PLOT_H/2, -20, tempUnit); //PLOT_H-30,






//    for(int i = 0; i < )
    delete painter;

}

void Widget::initUi()
{
    plotWidget = new QNCustomplot(this);
    plotWidget->setGeometry(PLOT_X, PLOT_Y, PLOT_W, PLOT_H);
    plotWidget->setBackground(QBrush(Qt::black));//设置plot背景
    plotWidget->xAxis->setRange(0, XAXIS_LEN);
    plotWidget->yAxis->setRange(0, YAXIS_LEN);

    plotWidget->xAxis->setAutoTickStep(false);
    plotWidget->xAxis->setTickStep(1.0);

    plotWidget->yAxis->setAutoTickStep(false);
    plotWidget->yAxis->setTickStep(YAXIS_LEN/6.0);

    plotWidget->xAxis->setTickLabels(false);
    plotWidget->yAxis->setTickLabels(false);

    plotWidget->xAxis->setBasePen(QPen(Qt::gray, 1));//x轴边线
    plotWidget->yAxis->setBasePen(QPen(Qt::gray, 1));//y轴边线
    plotWidget->xAxis->setTickPen(QPen(Qt::gray, 2));//边线中的大刻度
    plotWidget->yAxis->setTickPen(QPen(Qt::gray, 2));
    plotWidget->xAxis->setSubTickPen(QPen(Qt::lightGray, 1));//边线中的小刻度
    plotWidget->yAxis->setSubTickPen(QPen(Qt::lightGray, 1));



    barsAnalog = new QCPBars(plotWidget->xAxis,plotWidget->yAxis);
    barsAudio = new QCPBars(plotWidget->xAxis,plotWidget->yAxis);
    barsDVB = new QCPBars(plotWidget->xAxis,plotWidget->yAxis);
    barsDVB->setBrush(QBrush(Qt::green));
    barsAnalog->setBrush(QBrush(Qt::green));
    barsAudio->setBrush(QBrush(Qt::blue));

//    barsAnalog->setName("analog");

//    QMargins margin(20, 20, 100, 100);
//    plotWidget->setM
//    barsAnalog->setWidth(0.5);

//    plotWidget->registerPlottable(barsAnalog);
//    plotWidget->registerPlottable(barsAudio);

    replot();

    QPalette pal;
    pal.setColor(QPalette::WindowText, Qt::white);

    labelAnalog = new QLabel(this);
    labelAnalog->setPalette(pal);
    labelAnalog->setText("TV Level");
    labelAnalog->move(PLOT_X+55, PLOT_W);

    labelAudio = new QLabel(this);
    labelAudio->setText("Audio Level");
    labelAudio->setPalette(pal);
    labelAudio->move(PLOT_X+205, PLOT_W);

    labelDVB = new QLabel(this);
    labelDVB->setPalette(pal);
    labelDVB->setText("POWER");
    labelDVB->move(PLOT_X+143, PLOT_W);

    labelUpValue = new QLabel(this);
    labelUpValue->setPalette(pal);
    labelUpValue->setAlignment(Qt::AlignLeft);
    labelUpValue->setGeometry(0,0,120,30);

    labelDownValue = new QLabel(this);
    labelDownValue->setPalette(pal);
    labelDownValue->setAlignment(Qt::AlignLeft);
    labelDownValue->setGeometry(0,0,120,30);

    labelUpLimit = new QLabel(this);
    labelUpLimit->setGeometry(0, 100, PLOT_W - MARGIN_UP*2, 4);

    labelDownLimit = new QLabel(this);
    labelDownLimit->setGeometry(0, 200, PLOT_W - MARGIN_UP*2, 4);


    labelVALimit = new QLabel(this);
    labelVALimit->setGeometry(PLOT_X+MARGIN_UP+10, MARGIN_UP, 200, 30);
    labelVALimit->setAlignment(Qt::AlignLeft);
    labelVALimit->setPalette(pal);


    QPixmap pixmap;
    pixmap.load("://rc/limitLine.png");
    labelUpLimit->setPixmap(pixmap);
    labelDownLimit->setPixmap(pixmap);
    labelUpLimit->move(PLOT_X + PLOT_MARGIN, 100);
    labelDownLimit->move(PLOT_X + PLOT_MARGIN, 200);
    labelUpLimit->show();
    labelDownLimit->show();

    double limit[2] = {120, 120};
    setLimit(limit);
}


void Widget::initData()
{
    QString temp;
    for(int i = 0; i < BAR_COUNT; i++){
        temp.sprintf("BAR%d", i);
        labels.append(temp);
        values.append(0);
    }

    currentMode = ANALOG_TV;
}



void Widget::setLimit(double Limit[])
{
    QString temp;

    upLimitLine = Limit[0];
    downLimitLine = Limit[1];


    labelUpLimit->move(PLOT_X + PLOT_MARGIN,
                       (upLine - upLimitLine)*(PLOT_H-PLOT_MARGIN*2)/POWER_RANGE + MARGIN_UP);
    labelDownLimit->move(PLOT_X + PLOT_MARGIN,
                         (upLine - downLimitLine)*(PLOT_H-PLOT_MARGIN*2)/POWER_RANGE + MARGIN_UP);


    labelUpValue->setText(temp.sprintf("%.2f", upLimitLine));
    labelDownValue->setText(temp.sprintf("%.2f", downLimitLine));

    labelUpValue->move(PLOT_W,
                       (upLine - upLimitLine)*(PLOT_H-PLOT_MARGIN*2)/POWER_RANGE + MARGIN_UP);
    labelDownValue->move(PLOT_W,
                         (upLine - downLimitLine)*(PLOT_H-PLOT_MARGIN*2)/POWER_RANGE + MARGIN_UP);

}

void Widget::setCurrentMode(int nMode)
{
    currentMode = nMode;

    labelAnalog->show();
    labelAudio->show();
    labelDVB->show();
    labelVALimit->show();

    if(currentMode == ANALOG_TV){
        valueDVB = 0;
        labelDVB->hide();
    }else{
        valueAnalog = 0;
        valueAudio = 0;
        labelAnalog->hide();
        labelAudio->hide();
        labelVALimit->hide();
    }

    replot();
}

void Widget::setDVBPower(double value)
{
    if(currentMode != ANALOG_TV){
        valueDVB = value;
        replot();
    }
}

void Widget::setAnalogPower(double value)
{
    if(currentMode == ANALOG_TV){
        valueAnalog = value;
        replot();
    }
}

void Widget::setAudioPower(double value)
{
    if(currentMode == ANALOG_TV){
        valueAudio = value;
        replot();
    }
}

void Widget::UpdateCurrentUnit()
{
//    upLine = float(int(120 - unitChangeBiases[g_dbmUnit] + 1));
    switch(g_dbmUnit){
    case unitLimit_dBuv:
        upLine = 120;
        break;
    case unitLimit_dBmv:
        upLine = 60;
        break;
    case unitLimit_dBm:
        upLine = 12;
        break;
    default:
        upLine = 120;
        break;
    }
}

void Widget::replot()
{
    QVector<double> key(1);
    double tempValue;
    key[0]=1;
    values[0] = valueAnalog;
    barsAnalog->setData(key,values);
    barsAnalog->setBrush(QBrush(QColor(Qt::red)));
    tempValue = valueAnalog - unitChangeBiases[g_dbmUnit];
    if(tempValue <= upLimitLine && tempValue >= downLimitLine){
        barsAnalog->setBrush(QBrush(QColor(Qt::green)));
    }

    key[0]=3;
    values[0] = valueAudio;
    barsAudio->setData(key,values);
    barsAudio->setBrush(QBrush(QColor(Qt::red)));
    tempValue = valueAnalog - valueAudio;
    if(tempValue <= limitVA_show[0] && tempValue >= limitVA_show[1]){
        barsAudio->setBrush(QBrush(QColor(Qt::blue)));
    }

    key[0]=2;
    values[0] = valueDVB;
    barsDVB->setData(key, values);
    barsDVB->setBrush(QBrush(QColor(Qt::red)));
    tempValue = valueDVB - unitChangeBiases[g_dbmUnit];
    if(tempValue <= upLimitLine && tempValue >= downLimitLine){
        barsDVB->setBrush(QBrush(QColor(Qt::green)));
    }

    plotWidget->replot();
}

void Widget::testInit()
{
    testButton = new QPushButton(this);
    testButton->show();
//    testButton->hide();
    connect(testButton, SIGNAL(clicked()), this, SLOT(testFunction()));
}

void Widget::testFunction()
{
    static int cnt = 0;
    static double limit[2] = {0};
    limit[0] = 120;
    limit[1] += 20;
    if(limit[1] == 140){
        limit[1] = 0;
    }

//    cnt += 5;
//    if(cnt == 80)
//        cnt = 0;
//    valueAnalog = cnt;
//    valueAudio = cnt;
//    valueDVB = cnt;

    setLimit(limit);
    replot();
}
