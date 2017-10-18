#include "nlog.h"

char logInfo[250] = {'\0'};

NLog::NLog(QObject *parent) : QObject(parent)
{
    log_file_list = NULL;
    log_file_list = new QList<QFile *>;
}

int NLog::Initial(QString Path, int logClassCnt)
{
    int ret = -1;
    QDir dirCheck(Path);
    if(!dirCheck.exists()){
        mkpath(Path);
    }

    QFile *tempFile = NULL;
    for(int index = 0; index < logClassCnt; index++){
        Path = Path + QString("%1").arg(index);
        tempFile = new QFile(Path);
        log_file_list->append(tempFile);
        if(!tempFile->open(QIODevice::ReadWrite|QIODevice::Append))
            return ret;
    }

    return 0;
}

void NLog::logInfo(QString info, int classIndex)
{
    if(log_file_list == NULL)
        return ;
    QFile *logFile = log_file_list->at(classIndex);
    sprintf(logInfo, "%d:%d:%d: %s\n", QTime::currentTime().hour(), QTime::currentTime().minute(), QTime::currentTime().second(), \
            info.toStdString().c_str());

    logFile->write(logInfo);
}

NLog::~NLog()
{
    if(log_file_list != NULL){
        QFile *tempFile = NULL;
        while(log_file_list->count() > 0){
            tempFile = log_file_list->at(0);
            log_file_list->removeAt(0);
            delete tempFile;
        }
        delete log_file_list;
    }

}

int log_Initial(QString Path, int logClassCnt)
{
    nlog = new NLog;
    nlog->Initial(Path, logClassCnt);
}

void log_Info(QString strInof, int classIndex)
{
    nlog->logInfo(strInof, classIndex);
}

void log_end()
{
    delete nlog;
}
