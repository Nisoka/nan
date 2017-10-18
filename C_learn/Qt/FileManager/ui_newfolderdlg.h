/********************************************************************************
** Form generated from reading UI file 'newfolderdlg.ui'
**
** Created: Fri Dec 6 15:30:35 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_NEWFOLDERDLG_H
#define UI_NEWFOLDERDLG_H

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

class Ui_NewFolderDlg
{
public:
    QGroupBox *groupBox;
    QLineEdit *editFolder;
    QLabel *label;
    QPushButton *okBtn;
    QPushButton *cancelBtn;

    void setupUi(QDialog *NewFolderDlg)
    {
        if (NewFolderDlg->objectName().isEmpty())
            NewFolderDlg->setObjectName(QString::fromUtf8("NewFolderDlg"));
        NewFolderDlg->resize(279, 109);
        groupBox = new QGroupBox(NewFolderDlg);
        groupBox->setObjectName(QString::fromUtf8("groupBox"));
        groupBox->setGeometry(QRect(10, 10, 261, 61));
        editFolder = new QLineEdit(groupBox);
        editFolder->setObjectName(QString::fromUtf8("editFolder"));
        editFolder->setGeometry(QRect(110, 30, 141, 20));
        editFolder->setMaxLength(30);
        label = new QLabel(groupBox);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(20, 30, 81, 16));
        okBtn = new QPushButton(NewFolderDlg);
        okBtn->setObjectName(QString::fromUtf8("okBtn"));
        okBtn->setGeometry(QRect(120, 80, 75, 23));
        cancelBtn = new QPushButton(NewFolderDlg);
        cancelBtn->setObjectName(QString::fromUtf8("cancelBtn"));
        cancelBtn->setGeometry(QRect(200, 80, 75, 23));

        retranslateUi(NewFolderDlg);

        QMetaObject::connectSlotsByName(NewFolderDlg);
    } // setupUi

    void retranslateUi(QDialog *NewFolderDlg)
    {
        NewFolderDlg->setWindowTitle(QApplication::translate("NewFolderDlg", "Dialog", 0, QApplication::UnicodeUTF8));
        groupBox->setTitle(QApplication::translate("NewFolderDlg", "New Folder", 0, QApplication::UnicodeUTF8));
        label->setText(QApplication::translate("NewFolderDlg", "Folder Name:", 0, QApplication::UnicodeUTF8));
        okBtn->setText(QApplication::translate("NewFolderDlg", "OK", 0, QApplication::UnicodeUTF8));
        cancelBtn->setText(QApplication::translate("NewFolderDlg", "Cancel", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class NewFolderDlg: public Ui_NewFolderDlg {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_NEWFOLDERDLG_H
