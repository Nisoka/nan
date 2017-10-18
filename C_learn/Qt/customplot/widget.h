#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "qncustomplot.h"
#include "globalVar.h"


#define T_W 410
#define T_H 400

#define ANALOG_TV 1
#define DVB_C   2

namespace Ui {
class Widget;
}



class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();


    void UpdateLimit();

private:
    Ui::Widget *ui;

private:
    QNCustomplot *plotWidget;
    QCPBars* barsAnalog;
    QCPBars* barsAudio;
    QCPBars* barsDVB;
    QLabel* labelAnalog;
    QLabel* labelAudio;
    QLabel* labelDVB;

    QVector<QString> labels;
    QVector<double> values;
    double valueDVB;
    double valueAnalog;
    double valueAudio;
    QLabel* labelUpLimit;
    QLabel* labelDownLimit;
    QLabel* labelUpValue;
    QLabel* labelDownValue;

    QLabel* labelVALimit;

    double upLine;
    double upLimitLine;
    double downLimitLine;
    int currentMode;


protected:
    void paintEvent(QPaintEvent *event);

private:
    void initUi();
    void initData();

public:

    void setLimit(double Limit[]);

    void setCurrentMode(int nMode);
    void setDVBPower(double value);
    void setAnalogPower(double value);
    void setAudioPower(double value);
    void UpdateCurrentUnit();
    void replot();



    //TEST
public:
    QPushButton *testButton;
public slots:
    void testInit();
    void testFunction();

};

#endif // WIDGET_H
