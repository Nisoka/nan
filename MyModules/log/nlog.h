#ifndef NLOG_H
#define NLOG_H

#include "../QtHeads.h"

QList<QFile *> *log_file_list;

class NLog : public QObject
{
    Q_OBJECT
public:
    explicit NLog(QObject *parent = 0);
    ~NLog();

    int Initial(QString Path, int logClassCnt = 1);
    void logInfo(QString info, int classIndex);


signals:

public slots:
};



NLog *nlog = NULL;
int log_Initial(QString Path, int logClassCnt = 1);
void log_Info(QString strInof, int classIndex = 0);
void log_end();

#endif // NLOG_H
