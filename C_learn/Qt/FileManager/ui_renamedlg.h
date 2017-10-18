/********************************************************************************
** Form generated from reading UI file 'renamedlg.ui'
**
** Created: Fri Dec 6 15:30:35 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_RENAMEDLG_H
#define UI_RENAMEDLG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QGroupBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_RenameDlg
{
public:
    QGroupBox *groupBox;
    QLabel *label;
    QLineEdit *editName;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *RenameDlg)
    {
        if (RenameDlg->objectName().isEmpty())
            RenameDlg->setObjectName(QString::fromUtf8("RenameDlg"));
        RenameDlg->resize(269, 109);
        groupBox = new QGroupBox(RenameDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 251, 61));
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(40, 30, 41, 16));
        editName = new QLineEdit(groupBox);
        editName->setObjectName(QString::fromUtf8("editName"));
        editName->setGeometry(QRect(80, 30, 131, 20));
        editName->setMaxLength(30);
        okBtn = new QPushButton(RenameDlg);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));
        okBtn->setGeometry(QRect(100, 80, 75, 23));
        cancelBtn = new QPushButton(RenameDlg);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setGeometry(QRect(180, 80, 75, 23));

        retranslateUi(RenameDlg);

        QMetaObject::connectSlotsByName(RenameDlg);
    } // setupUi

    void retranslateUi(QDialog *RenameDlg)
    {
        RenameDlg->setWindowTitle(QApplication::translate("RenameDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("RenameDlg", "Rename", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("RenameDlg", "Name:", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("RenameDlg", "OK", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("RenameDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class RenameDlg: public Ui_RenameDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_RENAMEDLG_H
