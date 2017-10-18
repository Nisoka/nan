/********************************************************************************
** Form generated from reading UI file 'filemanagerform.ui'
**
** Created: Fri Dec 6 15:30:35 2013
**      by: Qt User Interface Compiler version 4.8.3
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEMANAGERFORM_H
#define UI_FILEMANAGERFORM_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHeaderView>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileForm
{
public:
    QWidget *centralWidget;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FileForm)
    {
        if (FileForm->objectName().isEmpty())
            FileForm->setObjectName(QString::fromUtf8("FileForm"));
        FileForm->resize(400, 300);
        centralWidget = new QWidget(FileForm);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        FileForm->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FileForm);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 400, 23));
        FileForm->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FileForm);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FileForm->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FileForm);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FileForm->setStatusBar(statusBar);

        retranslateUi(FileForm);

        QMetaObject::connectSlotsByName(FileForm);
    } // setupUi

    void retranslateUi(QMainWindow *FileForm)
    {
        FileForm->setWindowTitle(QApplication::translate("FileForm", "FileForm", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FileForm: public Ui_FileForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEMANAGERFORM_H
