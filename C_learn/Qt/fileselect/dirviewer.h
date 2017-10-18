#ifndef DIRVIEWER_H
#define DIRVIEWER_H

#include <QWidget>
#include <QFileSystemModel>
#include <QTreeView>
#include <QDirModel>


#include "maintainfun.h"
#include "../button.h"
#include "../mydelegate.h"

namespace Ui {
class DirViewer;
}


class DirViewer : public QWidget
{
    Q_OBJECT

public:
    explicit DirViewer(QWidget *parent = 0);
    ~DirViewer();

    enum{
        Accepted,
        Rejected
    };
    QString dirPath;
    QFileSystemModel *model;
    QTreeView *treeView;

    PushBtn *OpenButtonP;
//    PushBtn *SearchButtonP;
    PushBtn *ExitButtonP;

    QEventLoop * m_loop;
    int m_result;
    QString m_retPath;
private:
    Ui::DirViewer *ui;

private slots:
    void DealOpenFile(void);
    void DealFileDoubleClicked(QModelIndex);
signals:
    void msgExit();
    void msgOpenFile(QString);
public slots:
    void hide();
    void on_Exit();
    void SetDirPath(QString path);
    QString getSelectPath(){return m_retPath;}
    void open();
    int exec();
    void exec_exit();
};

#endif // DIRVIEWER_H
