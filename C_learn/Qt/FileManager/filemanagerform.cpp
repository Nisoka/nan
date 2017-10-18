#include "filemanagerform.h"
#include "ui_filemanagerform.h"
#include <QDir>
#include <QDebug>
#include <QDateTime>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QLayout>
#include <QKeyEvent>
#include <QCheckBox>
#include <QScrollBar>

//verticalScrollBar
//

FileForm::FileForm(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::FileForm)
{
    ui->setupUi(this);
    createAction();
    createToolBar();
    //字体设置
    QFont font("WenQuanYi Zen Hei");
    font.setPixelSize(14);
    font.setStyleStrategy(QFont::PreferQuality);
    QApplication::setFont(font);

    m_splitter = new QSplitter(Qt::Horizontal);
    m_fileTree = new QTreeView(m_splitter);
    m_fileList = new QTreeView(m_splitter);
    m_currentPath = PATH_DOC;
    //接收线程完毕信号
    connect(&m_fileProcess,SIGNAL(finished()),this,SLOT(fileProcessFinished()));
    m_sockPlug.bind();
    //得到U盘SD卡状态
    m_movStat = getMovStat();
    //初始多选状态
    m_multiCheck = false;
    m_selectAllFlag = false;

    initFileTree();
    initFileList();

    m_splitter->addWidget(m_fileTree);
    m_splitter->addWidget(m_fileList);
    m_splitter->setStretchFactor(1,1);
    setCentralWidget(m_splitter);

    setWindowFlags(windowFlags()|Qt::FramelessWindowHint);
    setGeometry(0,40,800,440);

    connect(&m_timerPlug,SIGNAL(timeout()),this,SLOT(checkPlug()));

#ifndef PC_DEBUG
    m_timerPlug.start(2000);
#endif
}

void FileForm::updateData()
{
    initFileTreeModal();
    initFileListModal();
}

void FileForm::checkPlug()
{
    int plugStat;
    plugStat = getMovStat();
    if(plugStat != m_movStat && isEnabled())
    {
        m_movStat = plugStat;
        initFileTreeModal();
        initFileListModal();
    }
}

void FileForm::showIndicator(bool show)
{
    m_indicator.setWindowFlags(m_indicator.windowFlags() | Qt::ToolTip |
                                Qt::FramelessWindowHint);
    if(show)
        m_indicator.show();
    else
        m_indicator.hide();
}

void FileForm::fileProcessFinished()
{
    if(CMD_COPY == m_fileProcess.getCurrentCmd())
    {
        updateTree(m_currentPath,m_copyPath.right(m_copyPath.count()-m_copyPath.lastIndexOf("/")-1));
        m_copyPath = "";
        loadFileEntry(m_currentPath, false);
    }
    else if(CMD_COPY_FILE == m_fileProcess.getCurrentCmd())
    {
        m_copyPath = "";
        loadFileEntry(m_currentPath, false);
    }
    else if(CMD_DEL == m_fileProcess.getCurrentCmd())
    {
        QModelIndex listIndex = m_fileList->currentIndex();
        if(!listIndex.isValid())
            return;
        QString delPath = listIndex.data(Qt::UserRole).toString();
        updateTree(delPath,"",false);
        loadFileEntry(m_currentPath, false);
    }
    else if(CMD_DELBULK == m_fileProcess.getCurrentCmd())
    {
        QStringList list = m_fileProcess.getFilePath();
        for(int i = 0; i < list.count(); i++)
        {
            updateTree(list[i],"",false);
        }

        //刷新文件列表
        loadFileEntry(m_currentPath, false);
        //刷新Selete All图标显示
        //m_selectAll->setIconText("Select All");
        m_unselect->setVisible(false);
        m_selectAll->setVisible(true);
    }
    else if(CMD_SELECT_ALL == m_fileProcess.getCurrentCmd())
    {
        m_multiCheck = true;
        m_selectAllFlag = true;
        //m_selectAll->setIconText("Unselect");
        m_selectAll->setVisible(false);
        m_unselect->setVisible(true);
    }
    else if(CMD_CLEAR_SEL == m_fileProcess.getCurrentCmd())
    {
        m_multiCheck = false;
        m_selectAllFlag = false;
        //m_selectAll->setIconText("Select All");
        m_unselect->setVisible(false);
        m_selectAll->setVisible(true);
    }
    else if(CMD_COPYBULK == m_fileProcess.getCurrentCmd())
    {
        QStringList list = m_fileProcess.getFilePath();
        for(int i = 0; i < list.count(); i++)
        {
            QFileInfo info(list[i]);
            if(info.isFile())
            {
                m_copyPath = "";
                loadFileEntry(m_currentPath, false);
            }
            else
            {
                m_copyPath = "";
                updateTree(m_currentPath,list[i].right(list[i].count()-list[i].lastIndexOf("/")-1));
                loadFileEntry(m_currentPath, false);
            }
        }
    }
 //   extractSubFolder(m_currentPath,item,1,true);
//#ifdef ARM_CODE
    system("sync");
//#endif
    showIndicator(false);
    setEnabled(true);
    if(m_fileProcess.getErrorCode() < 0)
    {
        QMessageBox mb(QMessageBox::Warning,tr("Warning"),tr("An error occur during operation!"),QMessageBox::Ok);
        mb.setWindowIcon(QIcon("://rc/image/icon.png"));
        mb.exec();
    }
}

void FileForm::upwards()
{
    QString path;
    if(m_currentPath != PATH_DOC && m_currentPath != PATH_SD &&
            m_currentPath != PATH_USB && m_currentPath != PATH_USB1)
    {
        path = m_currentPath.left(m_currentPath.lastIndexOf("/"));
    }
    else
    {
        return;
    }

    loadFileEntry(path, false);
}

void FileForm::fileSelectAll()
{
    int count = m_fileListModal.rowCount();

    if(count == 0)
        return ;

    //记录原来的状态
    int valueBak = m_fileList->verticalScrollBar()->value();

    if(!m_selectAllFlag)
    {
        //重新初始化文件列表
        loadFileEntry(m_currentPath, true);
        //恢复滚动条位置
        QModelIndex checkedIndex = m_fileListModal.index(valueBak+5, 1, QModelIndex());
        m_fileList->scrollTo(checkedIndex);

        //
        m_fileProcess.setCmd(CMD_SELECT_ALL);
        m_fileProcess.setSelectAllList(m_checkedList);
        m_fileProcess.start();
    }
    else
    {
        //重新初始化文件列表
        loadFileEntry(m_currentPath, false);
        //恢复滚动条位置
        QModelIndex checkedIndex = m_fileListModal.index(valueBak+5, 1, QModelIndex());
        m_fileList->scrollTo(checkedIndex);

        //
        m_fileProcess.setCmd(CMD_CLEAR_SEL);
        m_fileProcess.setSelectAllList(m_checkedList);
        m_fileProcess.start();
    }
}

void FileForm::fileOpen(void)
{       
    //
    QStringList list;
    GetCheckedFiles(&list);

    if(list.count() > 1)        //同时打开多条曲线
    {
        emit openMutiFiles(list);
    }
    else
    {
        QModelIndex index = m_fileList->currentIndex();
        if(index.isValid())
        {
            emit treeListDoubleClicked(index.data(Qt::UserRole).toString());
        }
    }
}

/*void FileForm::fileSelectAll()
{
    int count = m_fileListModal.rowCount();

    if(count == 0)
        return ;

    if(!m_selectAllFlag)
    {
        setEnabled(false);
        showIndicator(true);

        for(int i = 0 ;i < (count<8?count:8); i++)
        {
            QStandardItem *itemSelect = m_fileListModal.item(i, 0);
            itemSelect->setCheckState(Qt::Checked);
        }

        m_fileProcess.setCmd(CMD_SELECT_ALL);
        m_fileProcess.setSelectAllList(m_checkedList);
        m_fileProcess.start();
    }
    else
    {
        for(int i = 0 ;i < count ;i++)
        {
            QStandardItem *itemSelect = m_fileListModal.item(i, 0);
            itemSelect->setCheckState(Qt::Unchecked);

            m_checkedList.at(i)->checked = false;
        }

        m_multiCheck = false;
        m_selectAllFlag = false;

        m_selectAll->setIconText("Select All");
    }
}
*/

/*
void FileForm::fileSelectAll()
{
    int count = m_fileListModal.rowCount();

    if(count == 0)
        return ;

    if(!m_selectAllFlag)
    {
        for(int i = 0 ;i < count ;i++)
        {
            QStandardItem *itemSelect = m_fileListModal.item(i, 0);
            itemSelect->setCheckState(Qt::Checked);
        }
        m_multiCheck = true;
        m_selectAllFlag = true;

        m_selectAll->setIconText("Clear Sel");
    }
    else
    {
        for(int i = 0 ;i < count ;i++)
        {
            QStandardItem *itemSelect = m_fileListModal.item(i, 0);
            itemSelect->setCheckState(Qt::Unchecked);
        }
        m_multiCheck = false;
        m_selectAllFlag = false;

        m_selectAll->setIconText("Select All");
    }

}
*/

bool FileForm::getItemCheckStatus(const int row)
{
    //获取第 0 列的复选框的状态
    QStandardItem *itemSelect = m_fileListModal.item(row, 0);
    if(itemSelect->checkState() == Qt::Unchecked)
    {
        return false;
    }
    else
    {
        return true;
    }
}

void FileForm::fileDelSingle()
{
    QString path;
    QModelIndex index = m_fileList->currentIndex();
    if(!index.isValid())
        return;
    path = index.data(Qt::UserRole).toString();
    if(path.isEmpty()||path == PATH_DOC || path == PATH_SD || path == PATH_USB || path == PATH_USB1)
        return;
    QMessageBox mb(QMessageBox::Question,tr("Prompt"),tr("Do you want to delete this ?"),QMessageBox::Ok);
    mb.addButton(QMessageBox::Cancel);
    mb.setWindowIcon(QIcon("://rc/image/icon.png"));

    if(QMessageBox::Ok == mb.exec())
    {
        if(m_copyPath == path)
        {
            m_copyPath = "";
            m_paste->setEnabled(false);
        }
        setEnabled(false);
        showIndicator(true);

        m_fileProcess.setCmd(CMD_DEL);
        m_fileProcess.setDelFilePath(path);
        m_fileProcess.start();
    }
}

void FileForm::fileDelMultiple()
{
    int count = m_fileListModal.rowCount();
    if(count == 0)
        return ;

    QMessageBox mb(QMessageBox::Question,tr("Prompt"),tr("Do you want to delete these ?"),QMessageBox::Ok);
    mb.addButton(QMessageBox::Cancel);
    mb.setWindowIcon(QIcon("://rc/image/icon.png"));

    if(QMessageBox::Ok == mb.exec())
    {
        m_copyPath = "";
        m_paste->setEnabled(false);

        setEnabled(false);
        showIndicator(true);

        QStringList list;
        if(m_selectAllFlag)
        {
            for(int i = 0 ;i < count ; i++)
            {
                if(m_checkedList.at(i)->checked == true)
                {
                    list.append(m_checkedList.at(i)->path);
                }
            }
        }
        else
        {
            for(int i = 0 ;i < count ; i++)
            {
                if(getItemCheckStatus(i) == true)
                {
                    QModelIndex checkedIndex = m_fileListModal.index(i, 1, QModelIndex());
                    QString filePath = m_fileListModal.data(checkedIndex,Qt::UserRole).toString();
                    list.append(filePath);
                    list.append(m_checkedList.at(i)->path);
                }
            }
        }
        m_multiCheck = false;
        m_fileProcess.setCmd(CMD_DELBULK);
        m_fileProcess.setFilePath(list);
        m_fileProcess.start();
    }
}

void FileForm::fileDel()
{
    if(m_multiCheck == false)
    {
        fileDelSingle();
    }
    else
    {
        fileDelMultiple();
    }
}

void FileForm::GetCheckedFiles(QStringList *list)
{
    int count = m_fileListModal.rowCount();

    if(m_selectAllFlag)
    {
        for(int i = 0 ;i < count ;i++)
        {
            if(m_checkedList.at(i)->checked)
            {
                list->append(m_checkedList.at(i)->path);
            }
        }
    }
    else
    {
        for(int i = 0 ;i < count ;i++)
        {
            if(getItemCheckStatus(i) == true)
            {
                QModelIndex checkedIndex = m_fileListModal.index(i, 0, QModelIndex());
                QString filePath = m_fileListModal.data(checkedIndex,Qt::UserRole).toString();
                list->append(filePath);
            }
        }
    }


}

void FileForm::fileCopy()
{
    if(m_multiCheck == false)
    {
        QModelIndex index = m_fileList->currentIndex();
        if(!index.isValid())
            return;
        QString filePath = index.data(Qt::UserRole).toString();
        if(!filePath.isEmpty())
        {
            m_copyPath = filePath;
            m_paste->setEnabled(true);
        }
    }
    else
    {
        QStringList list;
        GetCheckedFiles(&list);
        if(list.count() > 0)
        {
            m_paste->setEnabled(true);
        }

        m_multiList = list;
    }
}

/*void FileForm::fileCopy()
{
    QModelIndex index = m_fileList->currentIndex();
    if(!index.isValid())
        return;
    QString filePath = index.data(Qt::UserRole).toString();
    if(!filePath.isEmpty())
    {
        m_copyPath = filePath;
        m_paste->setEnabled(true);
    }
}
*/

bool FileForm::checkPasteValidation(QString copyPath, QString pastePath)
{
    //路径相同
    QString copyDirName = copyPath.right(copyPath.length()-copyPath.lastIndexOf("/")-1);
    if(copyPath == pastePath + "/" + copyDirName)
    {
        return false;
    }
    //同名
    QFileInfo fileInfo(pastePath + "/" + copyDirName);
    if(fileInfo.exists())
    {
        return false;
    }
    //父在子目录中
    if(pastePath.contains(copyPath))
    {
        return false;
    }

    return true;
}

void FileForm::filePaste()
{
    //新的
    if(m_multiCheck == false)
    {
        filePasteSingle();
    }
    else
    {
        filePasteMultiple();
    }
}

void FileForm::filePasteMultiple()
{
    m_multiCheck = false;

    //检查路径合法性
    bool hasSlected = false;
    int userSelection = QMessageBox::Ok;
    QStringList bakList;
    for(int i = 0; i < m_multiList.count(); i++)
    {
        if(!checkPasteValidation(m_multiList[i], m_currentPath))
        {
            if(!hasSlected)
            {
                QMessageBox mb(QMessageBox::Question,tr("Prompt"),tr("Include illegal path. Do you wish to skip these files?"),QMessageBox::Ok);
                mb.addButton(QMessageBox::Cancel);
                mb.setWindowIcon(QIcon("://rc/image/icon.png"));

                //如果用户选择跳过，则把不合法的路径直接从链表中去掉
                if(QMessageBox::Ok == mb.exec())
                {
                    userSelection = QMessageBox::Ok;
                    bakList.append(m_multiList.at(i));
                }
                else
                {
                    //如果用户选择取消，则清空链表，不再粘贴
                    userSelection = QMessageBox::Cancel;
                    m_multiList.clear();
                    break;
                }

                hasSlected = true;
            }
            else
            {
                if(QMessageBox::Ok == userSelection)
                {
                    bakList.append(m_multiList.at(i));
                }
            }
        }
    }

    //从链表中移除路径不合法的文件
    if(bakList.count() > 0)
    {
        for(int i = 0; i < m_multiList.count(); )
        {
            for(int j = 0; j < bakList.count(); j++)
            {
                if(m_multiList.at(i) == bakList.at(j))
                {
                    m_multiList.removeOne(m_multiList.at(i));
                    i = 0;
                    break;
                }
                else
                {
                    if(j == bakList.count()-1)
                    {
                        i++;
                    }
                }
            }
        }
    }

    //if(m_multiList.count() > 0)
    {
        setEnabled(false);
        showIndicator(true);
        m_fileProcess.setCmd(CMD_COPYBULK);
        m_fileProcess.setFilePath(m_multiList);
        //有用的只有目标目录
        m_fileProcess.setCopyParam(m_copyPath,m_currentPath);
        m_fileProcess.start();
        m_paste->setEnabled(false);
    }
}

void FileForm::filePasteSingle()
{
    //文件
    QFileInfo info(m_copyPath);
    if(info.isFile())
    {
        if(m_copyPath == m_currentPath + "/" + info.fileName())
        {
            QMessageBox mb(QMessageBox::Warning,tr("Warning"),tr("Cannot paste here!"),QMessageBox::Ok);
            mb.setWindowIcon(QIcon("://rc/image/icon.png"));
            mb.exec();
            return;
        }
        setEnabled(false);
        showIndicator(true);
        m_fileProcess.setCmd(CMD_COPY_FILE);
        m_fileProcess.setCopyParam(m_copyPath,m_currentPath + "/" + info.fileName());
        m_fileProcess.start();
        m_paste->setEnabled(false);
        return;
    }
    //文件夹
    if(checkPasteValidation(m_copyPath,m_currentPath))
    {
        QString copyDirName = m_copyPath.right(m_copyPath.length()-m_copyPath.lastIndexOf("/")-1);
        setEnabled(false);
        showIndicator(true);
        m_fileProcess.setCmd(CMD_COPY);
        m_fileProcess.setCopyParam(m_copyPath,m_currentPath + "/" + copyDirName);
        m_fileProcess.start();

        m_paste->setEnabled(false);
    }
    else
    {
        QMessageBox mb(QMessageBox::Warning,tr("Warning"),tr("Cannot paste here!"),QMessageBox::Ok);
        mb.setWindowIcon(QIcon("://rc/image/icon.png"));
        mb.exec();
    }
}

/*void FileForm::filePaste()
{
    //文件
    QFileInfo info(m_copyPath);
    if(info.isFile())
    {
        if(m_copyPath == m_currentPath + "/" + info.fileName())
        {
            QMessageBox mb(QMessageBox::Warning,tr("Warning"),tr("Cannot paste here!"),QMessageBox::Ok);
            mb.setWindowIcon(QIcon("://rc/image/icon.png"));
            mb.exec();
            return;
        }
        setEnabled(false);
        showIndicator(true);
        m_fileProcess.setCmd(CMD_COPY_FILE);
        m_fileProcess.setCopyParam(m_copyPath,m_currentPath + "/" + info.fileName());
        m_fileProcess.start();
        m_paste->setEnabled(false);
        return;
    }
    //文件夹
    if(checkPasteValidation(m_copyPath,m_currentPath))
    {
        QString copyDirName = m_copyPath.right(m_copyPath.length()-m_copyPath.lastIndexOf("/")-1);
        setEnabled(false);
        showIndicator(true);
        m_fileProcess.setCmd(CMD_COPY);
        m_fileProcess.setCopyParam(m_copyPath,m_currentPath + "/" + copyDirName);
        m_fileProcess.start();

        m_paste->setEnabled(false);
    }
    else
    {
        QMessageBox mb(QMessageBox::Warning,tr("Warning"),tr("Cannot paste here!"),QMessageBox::Ok);
        mb.setWindowIcon(QIcon("://rc/image/icon.png"));
        mb.exec();
    }
}
*/


void FileForm::fileRename()
{
#ifndef PC_DEBUG
    //added by zbs
    QCopChannel::send("fcdesk", "IME", "SHOW");
#endif
    QModelIndex index = m_fileList->currentIndex();
    if(!index.isValid())
        return;
    QString pathName = index.data(Qt::UserRole).toString();
    m_renameDlg.setWindowFlags(m_renameDlg.windowFlags()&~Qt::WindowContextHelpButtonHint
                                              |Qt::MSWindowsFixedSizeDialogHint);
    m_renameDlg.setFixedSize(QSize(269,109));
    m_renameDlg.setWindowIcon(QIcon("://rc/image/icon.png"));
    m_renameDlg.setFileName(pathName.right(pathName.length() - pathName.lastIndexOf("/") - 1));
    if(!pathName.isEmpty())
    {
        if(QDialog::Accepted == m_renameDlg.exec())
        {
            QString newName = m_renameDlg.getFileName();
            QString oldName = pathName.right(pathName.length() - pathName.lastIndexOf("/") - 1);
            QDir dir(m_currentPath);
            if(dir.rename(oldName,newName))
            {
                loadFileEntry(m_currentPath, false);
                //加载树
                renameTree(oldName,newName);
            }
            else
            {
                QMessageBox mb(QMessageBox::Warning,tr("Warning"),tr("Failed to rename!"),QMessageBox::Ok);
                mb.setWindowIcon(QIcon("://rc/image/icon.png"));
                mb.exec();
            }
        }
    }
}

void FileForm::renameTree(QString oldName, QString newName)
{
    QModelIndex index = m_fileTree->currentIndex().sibling(m_fileTree->currentIndex().row(),0);
    if(!index.isValid())
        return;
    QStandardItem *item = m_fileTreeModal.itemFromIndex(index);
    int count = item->rowCount();
    for(int i = 0;i < count ;i++)
    {
        if(item->child(i)->text() == oldName)
        {
            item->child(i)->setText(newName);
            item->child(i)->setData(m_currentPath+"/"+newName,Qt::UserRole);
            item->sortChildren(0);
            break;
        }
    }
}

void FileForm::windowClose()
{
//    if(QMessageBox::Ok == QMessageBox::question(NULL,tr("Prompt"),tr("Do you want to quit?"),
//                                                QMessageBox::Ok,QMessageBox::Cancel))
    QMessageBox mb(QMessageBox::Question,tr("Prompt"),tr("Do you want to quit?"),QMessageBox::Ok);
    mb.addButton(QMessageBox::Cancel);
    mb.setWindowIcon(QIcon("://rc/image/icon.png"));
    if(QMessageBox::Ok == mb.exec())
    {
        close();
    }  
}

void FileForm::retranslateFileManagerUI()
{
    ui->retranslateUi(this);
}

void FileForm::quitFileManager()
{
    close();
}

void FileForm::createAction()
{
//    m_upwards = new QAction(QIcon("://rc/image/up.png"),tr("Upwards"),this);
//    connect(m_upwards,SIGNAL(triggered()),this,SLOT(upwards()));
    m_newFolder = new QAction(QIcon("://rc/image/newfolder.png"),tr("New Folder"),this);
    connect(m_newFolder,SIGNAL(triggered()),this,SLOT(newFolder()));
    m_newFolder->setShortcut(QKeySequence(Qt::Key_1));

    m_copy = new QAction(QIcon("://rc/image/copy.png"),tr("Copy"),this);
    m_copy->setShortcut(QKeySequence(Qt::Key_2));
    connect(m_copy,SIGNAL(triggered()),this,SLOT(fileCopy()));
//    m_cut = new QAction(QIcon("://rc/image/scissors.png"),tr("cut"),this);
    m_paste = new QAction(QIcon("://rc/image/paste.png"),tr("Paste"),this);
    m_paste->setShortcut(QKeySequence(Qt::Key_3));
    m_paste->setEnabled(false);
    connect(m_paste,SIGNAL(triggered()),this,SLOT(filePaste()));

    m_rename = new QAction(QIcon("://rc/image/rename.png"),tr("Rename"),this);
    m_rename->setShortcut(QKeySequence(Qt::Key_4));
    connect(m_rename,SIGNAL(triggered()),this,SLOT(fileRename()));

    m_del = new QAction(QIcon("://rc/image/del.png"),tr("Delete"),this);
    m_del->setShortcut(QKeySequence(Qt::Key_5));
    connect(m_del,SIGNAL(triggered()),this,SLOT(fileDel()));

    m_selectAll = new QAction(QIcon("://rc/image/selectall.png"), tr("Select All"), this);
    m_selectAll->setShortcut(QKeySequence(Qt::Key_6));
    connect(m_selectAll,SIGNAL(triggered()),this,SLOT(fileSelectAll()));

    m_unselect = new QAction(QIcon("://rc/image/selectall.png"), tr("Unselect"), this);
    m_unselect->setShortcut(QKeySequence(Qt::Key_7));
    m_unselect->setVisible(false);
    connect(m_unselect,SIGNAL(triggered()),this,SLOT(fileSelectAll()));

    m_open = new QAction(QIcon("://rc/image/open.png"), tr("Open"), this);
    m_open->setShortcut(QKeySequence(Qt::Key_8));
    m_open->setVisible(true);
    connect(m_open,SIGNAL(triggered()),this,SLOT(fileOpen()));

    m_sep1 = new QAction(this);
    m_sep1->setSeparator(true);
    m_sep2 = new QAction(this);
    m_sep2->setSeparator(true);
    m_sep3 = new QAction(this);
    m_sep3->setSeparator(true);

    m_close = new QAction(QIcon("://rc/image/close.png"),tr("Close"),this);
    m_close->setShortcut(QKeySequence(Qt::Key_Escape));
    connect(m_close,SIGNAL(triggered()),this,SLOT(windowClose()));
}

//建在当前文件夹
void FileForm::newFolder()
{
#ifndef PC_DEBUG
    //added by zbs
    QCopChannel::send("fcdesk", "IME", "SHOW");
#endif
    m_newFolderDlg.setWindowFlags(m_newFolderDlg.windowFlags()&~Qt::WindowContextHelpButtonHint
                                  |Qt::MSWindowsFixedSizeDialogHint);
    m_newFolderDlg.setFixedSize(QSize(279,109));
    m_newFolderDlg.setWindowIcon(QIcon("://rc/image/icon.png"));

    if(QDialog::Accepted == m_newFolderDlg.exec())
    {
        QDir dir(m_currentPath);
        if(dir.mkdir(m_newFolderDlg.getFolderName()))
        {
            loadFileEntry(m_currentPath, false);
            updateTree(m_currentPath,m_newFolderDlg.getFolderName());
//#ifdef ARM_CODE
            system("sync");
//#endif
        }
        else
        {
            //创建失败
            QMessageBox mb(QMessageBox::Warning,tr("Warning"),tr("Cannot create folder!"),QMessageBox::Ok);
            mb.setWindowIcon(QIcon("://rc/image/icon.png"));
            mb.exec();
        }
    }
}

void FileForm::updateTree(QString pathName, QString newFileName ,bool add)
{
    QModelIndex index = m_fileTree->currentIndex().sibling(m_fileTree->currentIndex().row(),0);
    if(!index.isValid())
        return;
    QStandardItem *item = m_fileTreeModal.itemFromIndex(index);
    if(!add)
    {
        int count = item->rowCount();
        for(int i = 0 ; i < count ; i++)
        {
            if(m_currentPath+"/"+item->child(i)->text()==pathName)
            {
                item->removeRow(i);
                break;
            }
        }
    }
    else
    {
        QStandardItem *newItem,*emptyItem;
        QList<QStandardItem *> itemList;
        QFileInfo info(pathName + "/" + newFileName);
        if(info.isDir())
           newItem = new QStandardItem(QIcon("://rc/image/Folder.png"),newFileName);
        else
           newItem = new QStandardItem(QIcon("://rc/image/file.png"),newFileName);
        emptyItem = new QStandardItem("");
        newItem->setFlags(newItem->flags() & ~Qt::ItemIsEditable);
        emptyItem->setFlags(emptyItem->flags() & ~Qt::ItemIsEditable);
        newItem->setData(pathName + "/" + newFileName,Qt::UserRole);
//        item->appendRow(newItem);
        itemList.append(newItem);
        itemList.append(emptyItem);
        item->appendRow(itemList);
        item->sortChildren(0);
        extractSubFolder(pathName + "/" + newFileName,newItem,1);
    }
}

void FileForm::createToolBar()
{
    ui->mainToolBar->setToolButtonStyle(Qt::ToolButtonTextUnderIcon);
    ui->mainToolBar->setIconSize(QSize(32,32));
    ui->mainToolBar->setFloatable(false);
    ui->mainToolBar->setMovable(false);
//    ui->mainToolBar->setStyleSheet("border:0;");

//    ui->mainToolBar->addAction(m_upwards);
    ui->mainToolBar->addAction(m_sep1);
    ui->mainToolBar->addAction(m_open);
    ui->mainToolBar->addAction(m_newFolder);
    ui->mainToolBar->addAction(m_copy);
//    ui->mainToolBar->addAction(m_cut);
    ui->mainToolBar->addAction(m_paste);
    ui->mainToolBar->addAction(m_rename);
    ui->mainToolBar->addAction(m_del);
    ui->mainToolBar->addAction(m_sep2);
    ui->mainToolBar->addAction(m_selectAll);
    ui->mainToolBar->addAction(m_unselect);    
    ui->mainToolBar->addAction(m_sep3);
    ui->mainToolBar->addAction(m_close);
}

//return 0:无设备 1:U盘 2:SD卡 3:U盘&SD卡
int FileForm::getMovStat()
{
#ifdef PC_DEBUG
    return 0;
#endif

    char buf[10] = {0};
    m_sockPlug.writeDatagram("SERVer:MOVable:STATus?\r\n",QHostAddress("127.0.0.1"),1234);
/*    while(!m_sockPlug.hasPendingDatagrams())
        ;*/
    int loop = 5;
    while(!m_sockPlug.hasPendingDatagrams()&&0 != --loop)
        ;
    if(0 != loop)
        m_sockPlug.readDatagram(buf,10);
    else
        return 0;
    return atoi(buf);
}

void FileForm::treeExpanded(const QModelIndex &index)
{
    QStandardItem *item = m_fileTreeModal.itemFromIndex(index);
    int count = item->rowCount();
    for(int i = 0;i < count ;i++)
    {
        extractSubFolder(item->child(i)->data(Qt::UserRole).toString(),item->child(i),1);
    }
}
//树只显示文件夹
void FileForm::extractSubFolder(QString pathName, QStandardItem *item, int layer/*=8*/,bool forced/*=false*/)
{
    if(0 == layer)
        return ;
    if(forced)
        item->removeRows(0,item->rowCount());
    //如果已经有子项了不需要重复加载    
    if(0 != item->rowCount())
        return ;

    //每次都重新加载
//    item->removeRows(0,item->rowCount());
    QDir dir(pathName);
    QFileInfoList fileInfos = dir.entryInfoList(QDir::AllDirs|QDir::NoDotAndDotDot|QDir::NoSymLinks);
    int count = fileInfos.count();
    for(int i = 0 ;i < count ;i++)
    {
        int layerCount = layer;
        if(fileInfos[i].isDir())
        {
            QStandardItem *subItem = new QStandardItem(QIcon("://rc/image/Folder.png")
                        ,fileInfos[i].fileName());
            QStandardItem *itemHolder = new QStandardItem("");
            QList<QStandardItem*> itemList;
            subItem->setFlags(subItem->flags() & ~Qt::ItemIsEditable);
            itemHolder->setFlags(itemHolder->flags()&~Qt::ItemIsEditable);
            itemList.append(subItem);
            itemList.append(itemHolder);
            item->appendRow(itemList);

            m_fileTreeModal.setData(m_fileTreeModal.indexFromItem(subItem),
                                 fileInfos[i].filePath(),Qt::UserRole);
            extractSubFolder(fileInfos[i].filePath(),subItem,--layerCount);
        }
        //不处理文件
/*        else
        {
            QStandardItem *subItemName = new QStandardItem(QIcon("://rc/image/file.png")
                        ,fileInfos[i].fileName());
            QStandardItem *itemHolder = new QStandardItem("");
            QList<QStandardItem*> itemList;
            subItemName->setFlags(subItemName->flags() & ~Qt::ItemIsEditable);
            itemHolder->setFlags(itemHolder->flags()&~Qt::ItemIsEditable);
            itemList.append(subItem);
            itemList.append(itemHolder);
            item->appendRow(itemList);
            m_fileTreeModal.setData(m_fileTreeModal.indexFromItem(subItemName),
                                 fileInfos[i].filePath(),Qt::UserRole);
        }*/
    }
}

void FileForm::initFileTreeModal()
{
    m_fileTreeModal.clear();
    m_fileTreeModal.setColumnCount(2);
    m_fileTreeModal.setHeaderData(0,Qt::Horizontal,tr("Folder"));
    m_fileTreeModal.setHeaderData(1,Qt::Horizontal,"");
    //文档项
    QStandardItem *item = new QStandardItem(QIcon("://rc/image/disk.png"),tr("Document"));
    QStandardItem *itemHolder = new QStandardItem("");
    QList<QStandardItem *>itemList;
    item->setFlags(item->flags() & ~Qt::ItemIsEditable);
    itemHolder->setFlags(itemHolder->flags() & ~Qt::ItemIsEditable);
    itemList.append(item);
    itemList.append(itemHolder);
//    m_fileTreeModal.appendRow(item);
    m_fileTreeModal.appendRow(itemList);
    m_fileTreeModal.setData(m_fileTreeModal.indexFromItem(item),PATH_DOC,Qt::UserRole);
    m_fileTree->setCurrentIndex(m_fileTreeModal.indexFromItem(item));
    extractSubFolder(PATH_DOC,item,1);
    //U盘1
    if(1 == m_movStat || 3 == m_movStat || 6 == m_movStat || 7 == m_movStat)
    {
        item = new QStandardItem(QIcon("://rc/image/disk.png"),tr("Udisk1"));
        itemHolder = new QStandardItem("");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        itemHolder->setFlags(itemHolder->flags()&~Qt::ItemIsEditable);
        QList<QStandardItem *>devList;
        devList.append(item);
        devList.append(itemHolder);

        m_fileTreeModal.appendRow(devList);
        m_fileTreeModal.setData(m_fileTreeModal.indexFromItem(item),PATH_USB,Qt::UserRole);
        extractSubFolder(PATH_USB,item,1);
    }
    //U盘2
    if(4 == m_movStat || 5 == m_movStat || 6 == m_movStat || 7 == m_movStat)
    {
        item = new QStandardItem(QIcon("://rc/image/disk.png"),tr("Udisk2"));
        itemHolder = new QStandardItem("");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        itemHolder->setFlags(itemHolder->flags()&~Qt::ItemIsEditable);
        QList<QStandardItem *>devList;
        devList.append(item);
        devList.append(itemHolder);

        m_fileTreeModal.appendRow(devList);
        m_fileTreeModal.setData(m_fileTreeModal.indexFromItem(item),PATH_USB1,Qt::UserRole);
        extractSubFolder(PATH_USB1,item,1);
    }
    //SD卡
    if(2 == m_movStat || 3 == m_movStat || 5 == m_movStat || 7 == m_movStat)
    {
        item = new QStandardItem(QIcon("://rc/image/disk.png"),tr("SD"));
        itemHolder = new QStandardItem("");
        item->setFlags(item->flags() & ~Qt::ItemIsEditable);
        itemHolder->setFlags(itemHolder->flags()&~Qt::ItemIsEditable);
        QList<QStandardItem *>devList;
        devList.append(item);
        devList.append(itemHolder);

        m_fileTreeModal.appendRow(devList);
        m_fileTreeModal.setData(m_fileTreeModal.indexFromItem(item),PATH_SD,Qt::UserRole);
        extractSubFolder(PATH_SD,item,1);
    }
}

void FileForm::loadFileEntry(QString pathName, bool checked)
{
    m_currentPath = pathName;
    m_fileListModal.clear();
    m_fileListModal.setColumnCount(4);

    m_fileListModal.setHeaderData(0,Qt::Horizontal,tr("Select"));
    m_fileListModal.setHeaderData(1,Qt::Horizontal,tr("Name"));
    m_fileListModal.setHeaderData(2,Qt::Horizontal,tr("Last Modified Time"));
    m_fileListModal.setHeaderData(3,Qt::Horizontal,tr("Size"));

    //清除selectAll链表
    m_checkedList.clear();

    QDir dir(pathName);
    QFileInfoList fileInfos = dir.entryInfoList(QDir::AllEntries|QDir::NoDotAndDotDot|QDir::NoSymLinks,
                                                QDir::DirsFirst );
    int count = fileInfos.count();
    for(int i = 0 ;i < count ;i++)
    {
        if(fileInfos[i].isDir())
        {
            QStandardItem *itemSelect, *item,*itemModyfy,*itemSize;
            QList<QStandardItem*> itemList;

            //itemSelect = new QStandardItem(QIcon("://rc/image/checkbox.png"), "");
            itemSelect = new QStandardItem();
            itemSelect->setCheckable(true);
            if(checked)
            {
                itemSelect->setCheckState(Qt::Checked);
            }
            else
            {
                itemSelect->setCheckState(Qt::Unchecked);
            }

            item = new QStandardItem(QIcon("://rc/image/Folder.png"), fileInfos[i].fileName());
            itemModyfy = new QStandardItem(fileInfos[i].lastModified().toString("yyyy/MM/dd hh:mm:ss"));
            itemSize = new QStandardItem("");

            itemSelect->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            itemSelect->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setCheckable(false);
            itemModyfy->setFlags(item->flags() & ~Qt::ItemIsEditable);
            itemSize->setFlags(item->flags() & ~Qt::ItemIsEditable);

            itemList.append(itemSelect);
            itemList.append(item);
            itemList.append(itemModyfy);
            itemList.append(itemSize);


            m_fileListModal.appendRow(itemList);

            m_fileListModal.setData(m_fileListModal.indexFromItem(itemSelect),fileInfos[i].filePath(),Qt::UserRole);
            m_fileListModal.setData(m_fileListModal.indexFromItem(item),fileInfos[i].filePath(),Qt::UserRole);
            m_fileListModal.setData(m_fileListModal.indexFromItem(itemModyfy),fileInfos[i].filePath(),Qt::UserRole);
            m_fileListModal.setData(m_fileListModal.indexFromItem(itemSize),fileInfos[i].filePath(),Qt::UserRole);

            //记录用户数据，主要用来判断是否被选中
            UserType *tmpData = new UserType;
            tmpData->path = fileInfos[i].filePath();
            tmpData->checked = false;
            m_checkedList.append(tmpData);
        }
        else
        {
            QStandardItem *itemSelect, *item,*itemModyfy,*itemSize;
            QList<QStandardItem*> itemList;

            //itemSelect = new QStandardItem(QIcon("://rc/image/checkbox.png"), "");
            itemSelect = new QStandardItem();
            itemSelect->setCheckable(true);
            if(checked)
            {
                itemSelect->setCheckState(Qt::Checked);
            }
            else
            {
                itemSelect->setCheckState(Qt::Unchecked);
            }


            item = new QStandardItem(QIcon("://rc/image/file.png"), fileInfos[i].fileName());
            itemModyfy = new QStandardItem(fileInfos[i].lastModified().toString("yyyy/MM/dd hh:mm:ss"));
            itemSize = new QStandardItem(QString("%1").arg(fileInfos[i].size()));

            itemSelect->setFlags(item->flags() | Qt::ItemIsUserCheckable);
            itemSelect->setFlags(item->flags() & ~Qt::ItemIsEditable);
            item->setFlags(item->flags() & ~Qt::ItemIsEditable);
            itemModyfy->setFlags(item->flags() & ~Qt::ItemIsEditable);
            itemSize->setFlags(item->flags() & ~Qt::ItemIsEditable);

            itemList.append(itemSelect);
            itemList.append(item);
            itemList.append(itemModyfy);
            itemList.append(itemSize);

            m_fileListModal.appendRow(itemList);

            m_fileListModal.setData(m_fileListModal.indexFromItem(itemSelect),fileInfos[i].filePath(),Qt::UserRole);
            m_fileListModal.setData(m_fileListModal.indexFromItem(item),fileInfos[i].filePath(),Qt::UserRole);
            m_fileListModal.setData(m_fileListModal.indexFromItem(itemModyfy),fileInfos[i].filePath(),Qt::UserRole);
            m_fileListModal.setData(m_fileListModal.indexFromItem(itemSize),fileInfos[i].filePath(),Qt::UserRole);

            UserType *tmpData = new UserType;
            tmpData->path = fileInfos[i].filePath();
            tmpData->checked = false;
            m_checkedList.append(tmpData);
        }
    }
}

/*void MainWindow::loadFileEntry(QModelIndex &index)
{
    int count = m_fileTreeModal.rowCount(index);
    m_fileListModal.clear();
    m_fileListModal.setColumnCount(3);
    m_fileListModal.setHeaderData(0,Qt::Horizontal,tr("Name"));
    m_fileListModal.setHeaderData(1,Qt::Horizontal,tr("Last Modyfied Time"));
    m_fileListModal.setHeaderData(2,Qt::Horizontal,tr("Size"));
    //普通文件
    if(0 == count)
    {
        QStandardItem *item,*itemModyfy,*itemSize;
        QString pathName = m_fileTreeModal.data(index,Qt::UserRole).toString();
        QFileInfo fileInfo(pathName);
        QList<QStandardItem*> itemList;
        item = new QStandardItem(QIcon("://rc/image/file.png"),fileInfo.fileName());
        item->setData(fileInfo.filePath(),Qt::UserRole);

        itemModyfy = new QStandardItem(fileInfo.lastModified().toString("yyyy/MM/dd hh:mm:ss"));
        itemSize = new QStandardItem(QString("%1KB").arg(fileInfo.size()));
        itemList.append(item);
        itemList.append(itemModyfy);
        itemList.append(itemSize);
        m_fileListModal.appendRow(itemList);
    }

    for(int i = 0 ;i < count ;i ++)
    {
        QString pathName = m_fileTreeModal.data(index.child(i,0),Qt::UserRole).toString();
        QFileInfo fileInfo(pathName);
        QStandardItem *item,*itemModyfy,*itemSize;
        //处理不存在时的异常
        if(!fileInfo.exists())
            continue;
        if(fileInfo.isDir())
        {
            QList<QStandardItem*> itemList;
            item = new QStandardItem(QIcon("://rc/image/Folder.png"),fileInfo.fileName());
            item->setData(fileInfo.filePath(),Qt::UserRole);

            itemModyfy = new QStandardItem(fileInfo.lastModified().toString("yyyy/MM/dd hh:mm:ss"));
            itemList.append(item);
            itemList.append(itemModyfy);

            m_fileListModal.appendRow(itemList);
        }
        else if(fileInfo.isFile())
        {
            QList<QStandardItem*> itemList;
            item = new QStandardItem(QIcon("://rc/image/file.png"),fileInfo.fileName());
            item->setData(fileInfo.filePath(),Qt::UserRole);

            itemModyfy = new QStandardItem(fileInfo.lastModified().toString("yyyy/MM/dd hh:mm:ss"));
            itemSize = new QStandardItem(QString("%1KB").arg(fileInfo.size()));
            itemList.append(item);
            itemList.append(itemModyfy);
            itemList.append(itemSize);
            m_fileListModal.appendRow(itemList);
        }
        else
        {
            continue;
        }
    }
}
*/
void FileForm::initFileListModal()
{
    QModelIndex index = m_fileTreeModal.index(0,0);
    QString filePath = m_fileTreeModal.data(index,Qt::UserRole).toString();
    loadFileEntry(filePath, false);
//    loadFileEntry(index);
}

void FileForm::treeClicked(const QModelIndex &index)
{
    QModelIndex selectedIndex = index.sibling(index.row(),0);
    QString filePath = m_fileTreeModal.data(selectedIndex,Qt::UserRole).toString();
    loadFileEntry(filePath, false);
}

void FileForm::fileClicked(const QModelIndex &index)
{
    int selectCol = index.column();
    int selectRow = index.row();

    //如果点击了复选框
    if(selectCol == 0)
    {
        int count = m_fileListModal.rowCount();
        QStandardItem *itemSelect = m_fileListModal.item(selectRow, selectCol);
        //切换选择状态
        if(itemSelect->checkState() == Qt::Unchecked)
        {
            itemSelect->setCheckState(Qt::Checked);
            m_multiCheck = true;

            QString path = m_fileListModal.data(index,Qt::UserRole).toString();
            //qDebug() << "path = " << path;

            for(int i = 0; i < count; i++)
            {
                if(m_fileListModal.data(index, Qt::UserRole).toString() == m_checkedList.at(i)->path)
                {
                    if(m_checkedList.at(i)->checked == false)
                    {
                        m_checkedList.at(i)->checked = true;
                        //qDebug() << "set to true !";
                    }
                }
            }
            //如果都被选中了
            int i = 0;
            for(i = 0; i < count; i++)
            {
                if(m_checkedList.at(i)->checked == false)
                {
                    break;
                }
            }
            if(i == count)
            {
                m_selectAllFlag = true;
                m_unselect->setVisible(true);
                m_selectAll->setVisible(false);
            }
        }
        else
        {
            itemSelect->setCheckState(Qt::Unchecked);

            //如果原来是全选
            if(m_selectAllFlag)
            {
                m_selectAllFlag = false;
                //m_selectAll->setIconText("Select All");
                m_unselect->setVisible(false);
                m_selectAll->setVisible(true);
            }

            QString path = m_fileListModal.data(index,Qt::UserRole).toString();
            //qDebug() << "path = " << path;
            for(int i = 0; i < count; i++)
            {
                if(m_fileListModal.data(index, Qt::UserRole).toString() == m_checkedList.at(i)->path)
                {
                    if(m_checkedList.at(i)->checked == true)
                    {
                        m_checkedList.at(i)->checked = false;
                        //qDebug() << "set to false !";
                    }
                }
            }

            //如果都被取消了
            int i = 0;
            for(i = 0; i < count; i++)
            {
                if(m_checkedList.at(i)->checked == true)
                {
                    break;
                }
            }
            if(i == count)
            {
                m_multiCheck = false;
            }
        }
    }
}

bool FileForm::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == m_fileTree)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = (QKeyEvent*)event;
            if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                if(m_fileTree->isExpanded(m_fileTree->currentIndex().sibling(m_fileTree->currentIndex().row(),0)))
                    m_fileTree->collapse(m_fileTree->currentIndex().sibling(m_fileTree->currentIndex().row(),0));
                else
                    m_fileTree->expand(m_fileTree->currentIndex().sibling(m_fileTree->currentIndex().row(),0));
                return true;
            }
        }
    }
    else if(obj == m_fileList)
    {
        if(event->type() == QEvent::KeyPress)
        {
            QKeyEvent *keyEvent = (QKeyEvent*)event;
            if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return)
            {
                QModelIndex index = m_fileList->currentIndex();
                if(index.isValid())
                {
                    emit treeListDoubleClicked(index.data(Qt::UserRole).toString());
                    return true;
                }
            }
        }
    }
    return QObject::eventFilter(obj,event);
}

void FileForm::treeCurrentChange( const QModelIndex & current, const QModelIndex & previous)
{
    m_selectAllFlag = false;
    //m_multiCheck = false;
    //m_selectAll->setIconText("Select All");
    m_unselect->setVisible(false);
    m_selectAll->setVisible(true);

    QModelIndex selectedIndex = current.sibling(current.row(),0);
    QString filePath = m_fileTreeModal.data(selectedIndex,Qt::UserRole).toString();
    loadFileEntry(filePath, false);
}

void FileForm::initFileTree()
{
    initFileTreeModal();
    //文件数状态设置
    m_fileTree->header()->setResizeMode(QHeaderView::ResizeToContents);
    m_fileTree->header()->setMovable(false);
//    m_fileTree->header()->setStretchLastSection(false);
    m_fileTree->setFocus();
    m_fileTree->setSelectionMode(QAbstractItemView::SingleSelection);
    connect(m_fileTree,SIGNAL(expanded(QModelIndex)),this,SLOT(treeExpanded(QModelIndex)));
//    connect(m_fileTree,SIGNAL(clicked(QModelIndex)),this,SLOT(treeClicked(QModelIndex)));
    m_fileTree->installEventFilter(this);
    m_fileTree->setIconSize(QSize(64,46));
    m_fileTree->setModel(&m_fileTreeModal);
    connect(m_fileTree->selectionModel(),SIGNAL(currentChanged(const QModelIndex &,const QModelIndex &)),
            this,SLOT(treeCurrentChange(const QModelIndex &,const QModelIndex &)));
}

void FileForm::treeDoubleClicked(const QModelIndex &index)
{
    emit treeListDoubleClicked(index.data(Qt::UserRole).toString());
//    QString filePath = m_fileListModal.data(index,Qt::UserRole).toString();
//    loadFileEntry(filePath);
}

void FileForm::scrollValueChanged(int value)
{
    //qDebug() << "scroll value changed !" ;
    int rowCount = m_fileListModal.rowCount();
    int rowStart = m_fileList->verticalScrollBar()->value()+1;
    int rowCheckedEnd = rowStart + 7;

    if(rowCheckedEnd > rowCount)
    {
        rowCheckedEnd = rowCount;
    }

    for(int i=rowStart-1; i<rowCheckedEnd; i++)
    {
        if(m_checkedList.at(i)->checked)
        {
            QStandardItem *itemSelect = m_fileListModal.item(i, 0);
            itemSelect->setCheckState(Qt::Checked);
        }
    }
}

void FileForm::initFileList()
{
   initFileListModal();
   connect(m_fileList,SIGNAL(doubleClicked(QModelIndex)),this,SLOT(treeDoubleClicked(QModelIndex)));
   connect(m_fileList,SIGNAL(clicked(QModelIndex)),this,SLOT(fileClicked(QModelIndex)));
   //connect(m_fileList->verticalScrollBar(),SIGNAL(valueChanged(int)), this, SLOT(scrollValueChanged(int)));
   m_fileList->setIconSize(QSize(64,64));
   m_fileList->setModel(&m_fileListModal);
   m_fileList->header()->setResizeMode(QHeaderView::ResizeToContents );
   m_fileList->header()->setMovable(false);
   m_fileList->installEventFilter(this);
}

FileForm::~FileForm()
{
    delete ui;
}
