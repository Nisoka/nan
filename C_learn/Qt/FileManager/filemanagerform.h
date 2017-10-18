#ifndef FILEMANAGERFORM_H
#define FILEMANAGERFORM_H

#include <QMainWindow>
#include <QSplitter>
#include <QTreeView>
#include <QStandardItemModel>
#include <QAction>
#include "newfolderdlg.h"
#include "filemanagerthread.h"
#include "renamedlg.h"
#include "waitindicator.h"
#include <QUdpSocket>
#include <QTimer>
#include <QItemSelectionModel>
#include <QCopChannel>
//#define PC_DEBUG        1

#ifdef PC_DEBUG
#define PATH_DOC    "F:\TestDir"
#else
#define PATH_DOC    "/opt/user/document"
#endif

#define PATH_USB    "/mnt/udisk"
#define PATH_USB1    "/mnt/udisk1"
#define PATH_SD     "/mnt/sdcard"


namespace Ui {
class FileForm;
}


class FileForm : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit FileForm(QWidget *parent = 0);
    ~FileForm();
    void quitFileManager();
    void retranslateFileManagerUI();
    void updateData();
protected:
    bool eventFilter(QObject *obj, QEvent *event);
signals:
    void treeListDoubleClicked(QString data);
    void openMutiFiles(QStringList list);
private slots:
    void treeExpanded(const QModelIndex & index);
    void treeClicked(const QModelIndex & index);
    void fileClicked(const QModelIndex & index);
    void treeDoubleClicked( const QModelIndex & index);
    void GetCheckedFiles(QStringList *list);
    void upwards();
    void newFolder();
    void fileDel();
    void fileCopy();
    void filePaste();
    void filePasteSingle();
    void filePasteMultiple();
    void fileRename();
    void fileSelectAll();
    void fileOpen();
    void fileProcessFinished();
    void windowClose();
    void checkPlug();
    void treeCurrentChange( const QModelIndex & current, const QModelIndex & previous);
    void scrollValueChanged(int value);

private:
    void createAction();
    void createToolBar();
    void initFileTree();
    void initFileList();
    void initFileTreeModal();
    void initFileListModal();
    void showIndicator(bool show);
    bool getItemCheckStatus(const int row);
//    void loadFileEntry(QModelIndex &index);
    void loadFileEntry(QString pathName, bool checked);
    int getMovStat();
    void extractSubFolder(QString pathName,QStandardItem *item,int layer = 8,bool forced = false);
    void updateTree(QString pathName,QString newFileName,bool add = true);
    void renameTree(QString oldName,QString newName);
    bool checkPasteValidation(QString copyPath,QString pastePath);
    void fileDelSingle();
    void fileDelMultiple();
private:
    Ui::FileForm *ui;
    QSplitter *m_splitter;
    QTreeView *m_fileTree;
    QTreeView *m_fileList;
    QStandardItemModel m_fileTreeModal;
    QStandardItemModel m_fileListModal;
//    QAction *m_upwards;
    QAction *m_newFolder;
    QAction *m_copy;
//    QAction *m_cut;
    QAction *m_paste;
    QAction *m_rename;
    QAction *m_del;
    QAction *m_sep1;
    QAction *m_sep2;
    QAction *m_sep3;
    QAction *m_close;
    QAction *m_selectAll;
    QAction *m_unselect;
    QAction *m_open;
    int m_movStat;
    bool m_multiCheck;
    bool m_selectAllFlag;
    QString m_currentPath;
    QString m_copyPath;
    QStringList m_multiList;

    NewFolderDlg m_newFolderDlg;
    FileManagerThread m_fileProcess;
    RenameDlg m_renameDlg;
    WaitIndicator m_indicator;
    QUdpSocket m_sockPlug;
    QTimer m_timerPlug;
    QList <UserType *> m_checkedList;   
};

#endif // FILEMANAGERFORM_H
















