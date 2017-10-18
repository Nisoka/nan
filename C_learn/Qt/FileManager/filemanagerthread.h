#ifndef FILEMANAGERTHREAD_H
#define FILEMANAGERTHREAD_H

#include <QObject>
#include <QThread>
#include <QString>
#include <QStringList>


#define CMD_DEL         1
#define CMD_COPY        2
#define CMD_DELBULK     3
#define CMD_COPY_FILE   4
#define CMD_COPYBULK    5
#define CMD_SELECT_ALL  6
#define CMD_CLEAR_SEL   7

typedef struct userData {
    QString path;
    bool    checked;
} UserType;

class FileManagerThread : public QThread
{
    Q_OBJECT
public:
    explicit FileManagerThread(QObject *parent = 0);
    void setCmd(int cmd);
    void setDelFilePath(QString path);
    void setSelectAllList(QList <UserType *> list);
    void setCopyParam(QString source ,QString dest);
    int getCurrentCmd();
    void setFilePath(QStringList list);
    QStringList getFilePath();
    int getErrorCode();
private:
    bool removeDir(QString dirName);
    bool cpDir(QString srcPath,QString dstPath);
    bool removeDirBulk(QStringList list);
    bool cpFile(QString srcPath,QString dstPath);
    bool cpFileBulk(QStringList list);
    bool pasteFile();
    bool selectAllFiles(QList <UserType *> list);
    bool clearSelectAll(QList <UserType *> list);
protected:
    virtual void run();
signals:
    
public slots:
    
private:
    QString m_delFilePath;
    QList <UserType *> m_selectAllList;
    QString m_source;
    QString m_dest;
    QStringList m_fileList;
    int m_cmd;
    int m_err;
};

#endif // FILEMANAGERTHREAD_H
