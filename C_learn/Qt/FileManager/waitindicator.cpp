#include "waitindicator.h"
#include "ui_waitindicator.h"
#include <QLabel>
#include <QMovie>
#include <QDesktopWidget>
#include <QDebug>

WaitIndicator::WaitIndicator(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::WaitIndicator)
{
    ui->setupUi(this);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    setPalette(pal);
    QMovie *movie =new QMovie("://rc/image/loading.gif");
    ui->lableWaiting->setMovie(movie);
    movie->start();
    setGeometry((QApplication::desktop()->width()-size().width())/2,
                            (QApplication::desktop()->height()-size().height())/2,
                            size().width(),size().height());
}

WaitIndicator::~WaitIndicator()
{
    delete ui;
}


