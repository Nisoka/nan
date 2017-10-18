#include "ninfobox.h"

NInfoBox::NInfoBox(QWidget *parent, int Length, int Width, int Mergen) : QWidget(parent)
{
    this->setWindowModality(Qt::ApplicationModal);
    setMinimumSize(Length, Width);
    setMaximumSize(Length, Width);
    setWindowFlags(Qt::FramelessWindowHint);

    QPalette pal(this->palette());
    pal.setColor(QPalette::Background, QColor(Qt::blue)); //设置背景黑色
    this->setAutoFillBackground(true);
    this->setPalette(pal);




    m_pWidget = new QWidget(this);
    m_pWidget->setGeometry(Mergen , Mergen , Length - Mergen, Width - Mergen);

    QPalette pal2(m_pWidget->palette());
    pal2.setColor(QPalette::Background, QColor(Qt::white));//设置前景色
    m_pWidget->setAutoFillBackground(true);
    m_pWidget->setPalette(pal2);
    m_pWidget->show();
}



int easyMessageBox(QString info)
{
    QMessageBox msgBox;
    msgBox.setWindowFlags(Qt::FramelessWindowHint);
    msgBox.setText(info);
    msgBox.setIcon(QMessageBox::Information);
    msgBox.setWindowIcon(QIcon(":/info.ico"));
    if(msgBox.exec() == QMessageBox::Ok)
        return -1;

    return 0;
}

