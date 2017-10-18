#include "filemanagerthread.h"
#include <QFileInfo>
#include <QDir>
#include <QFileInfo>
#include <QDebug>


FileManagerThread::FileManagerThread(QObject *parent) :
    QThread(parent)
{
    m_cmd = 0;
}

int FileManagerThread::getErrorCode()
{
    return m_err;
}

void FileManagerThread::run()
{
    switch(m_cmd)
    {
        case CMD_DEL:
            if(!removeDir(m_delFilePath))
                m_err = -1;
            else
                m_err = 0;
            break;
        case CMD_COPY_FILE:
            if(!cpFile(m_source,m_dest))
                m_err = -1;
            else
                m_err = 0;
            break;
        case CMD_COPY:
            if(!cpDir(m_source,m_dest))
                m_err = -1;
            else
                m_err = 0;
            break;
        case CMD_COPYBULK:
            if(!cpFileBulk(m_fileList))
                m_err = -1;
            else
                m_err = 0;
            break;
        case CMD_DELBULK:
            if(!removeDirBulk(m_fileList))
                m_err = -1;
            else
                m_err = 0;
            break;
        case CMD_SELECT_ALL:
            if(!selectAllFiles(m_selectAllList))
                m_err = -1;
            else
                m_err = 0;
            break;
        case CMD_CLEAR_SEL:
            if(!clearSelectAll(m_selectAllList))
                m_err = -1;
            else
                m_err = 0;
            break;
    }
}

bool FileManagerThread::selectAllFiles(QList <UserType *> list)
{
    for(int i = 0; i < list.count(); i++)
    {
        list.at(i)->checked = true;
    }

    return true;
}

bool FileManagerThread::clearSelectAll(QList <UserType *> list)
{
    for(int i = 0; i < list.count(); i++)
    {
        list.at(i)->checked = false;
    }

    return true;
}


bool FileManagerThread::cpFileBulk(QStringList list)
{
    bool result = true;
    int count = list.count();
    for(int i = 0; i < count; i++)
    {
        QFileInfo info(list[i]);
        QString fileName = list[i].right(list[i].length() - list[i].lastIndexOf("/") - 1);
        QString curDest = m_dest + "/" + fileName;

        if(info.isFile())
        {
            if(!cpFile(list[i],curDest))
               result = false;
            else
               result = true;
        }
        else
        {
            if(!cpDir(list[i],curDest))
                result = false;
             else
                result = true;
        }
    }

    return result;
}

bool FileManagerThread::removeDirBulk(QStringList list)
{
    int count = list.count();

    for(int i = 0;i < count ;i++)
    {
        removeDir(list[i]);
    }
    return true;
}

void FileManagerThread::setFilePath(QStringList list)
{
    m_fileList = list;
}

QStringList FileManagerThread::getFilePath()
{
    return m_fileList;
}

bool FileManagerThread::removeDir(QString dirName)
{
    bool result = true;

    QFileInfo info(dirName);
    if(info.isFile())
    {
        return QFile::remove(dirName);
    }

    QDir dir(dirName);

    if (dir.exists(dirName)) {
        Q_FOREACH(QFileInfo info, dir.entryInfoList(QDir::NoDotAndDotDot
                                                    | QDir::AllEntries)) {
            if (info.isDir()) {
                result = removeDir(info.absoluteFilePath());
            }
            else {
                result = QFile::remove(info.absoluteFilePath());
            }

            if (!result) {
                return result;
            }
        }
        result = dir.rmdir(dirName);
    }

    return result;
}

bool FileManagerThread::cpFile(QString srcPath, QString dstPath)
{
    return QFile::copy(srcPath,dstPath);
}

bool FileManagerThread::cpDir(QString srcPath,QString dstPath)
{
    QFileInfo file(srcPath);
    if(file.isFile())
    {
        return QFile::copy(srcPath,dstPath + "/" + file.fileName());
    }

    QDir parentDstDir(QFileInfo(dstPath).path());
    if (!parentDstDir.mkdir(QFileInfo(dstPath).fileName()))
        return false;

    QDir srcDir(srcPath);
    foreach(const QFileInfo &info, srcDir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot)) {
        QString srcItemPath = srcPath + "/" + info.fileName();
        QString dstItemPath = dstPath + "/" + info.fileName();
        if (info.isDir()) {
            if (!cpDir(srcItemPath, dstItemPath)) {
                return false;
            }
        } else if (info.isFile()) {
            if (!QFile::copy(srcItemPath, dstItemPath)) {
                return false;
            }
        } else {

        }
    }
    return true;
}

void FileManagerThread::setCmd(int cmd)
{
    m_cmd = cmd;
}

void FileManagerThread::setDelFilePath(QString path)
{
    m_delFilePath = path;
}

void FileManagerThread::setSelectAllList(QList <UserType *> list)
{
    m_selectAllList = list;
}

void FileManagerThread::setCopyParam(QString source, QString dest)
{
    m_source = source;
    m_dest = dest;
}

int FileManagerThread::getCurrentCmd()
{
    return m_cmd;
}
















