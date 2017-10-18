/********************************************************************************
** Form generated from reading UI file 'waitindicator.ui'
**
** Created: Fri Dec 6 15:30:35 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_WAITINDICATOR_H
#define UI_WAITINDICATOR_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_WaitIndicator
{
public:
    QLabel *lableWaiting;

    void setupUi(QWidget *WaitIndicator)
    {
        if (WaitIndicator->objectName().isEmpty())
            WaitIndicator->setObjectName(QString::fromUtf8("WaitIndicator"));
        WaitIndicator->resize(550, 400);
        lableWaiting = new QLabel(WaitIndicator);
        lableWaiting->setObjectName(QString::fromUtf8("lableWaiting"));
        lableWaiting->setGeometry(QRect(0, 0, 550, 400));

        retranslateUi(WaitIndicator);

        QMetaObject::connectSlotsByName(WaitIndicator);
    } // setupUi

    void retranslateUi(QWidget *WaitIndicator)
    {
        WaitIndicator->setWindowTitle(QApplication::translate("WaitIndicator", "Form", 0, QApplication::UnicodeUTF8));
        lableWaiting->setText(QApplication::translate("WaitIndicator", "TextLabel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class WaitIndicator: public Ui_WaitIndicator {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_WAITINDICATOR_H
