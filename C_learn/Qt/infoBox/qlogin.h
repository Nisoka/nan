#ifndef QLOGIN_H
#define QLOGIN_H

#include <QWidget>
#include <QProgressBar>
#include <QTimer>
#include <QLabel>
#include <QPushButton>

#define QAM_VERSION_1 1
#define QAM_VERSION_2 0
#define QAM_VERSION_3 13

class QLogin : public QWidget
{
    Q_OBJECT
public:
    explicit QLogin(QWidget *parent = NULL);
    ~QLogin();

public:
    void startLogin();
private:
    QLabel *labelInfo;
    QProgressBar* progressBar;
    QTimer *timer;
    int progressCnt;

    QWidget *m_pWidget;

public slots:
    void waitting();

signals:
    void LoginOver();

};


class QAbout : public QWidget
{
    Q_OBJECT
public:
    explicit QAbout(QWidget* parent = NULL);
    ~QAbout();

    void setFirmwareVersion(int Version1 , int Version2 , int Version3);
    void setHardWareVersion(int Version1 , int Version2 , int Version3);

    QPushButton *btnConfirm;
    QLabel *labelInfo1;
    QLabel *labelInfo2;
    QLabel *labelInfo3;
    QLabel *labelVersionFirmware;
    QLabel *labelVersionHardWare;
    QLabel *labelVersionQAM;
    QWidget *m_pWidget;
};

class QInfoBox : public QWidget
{
    Q_OBJECT
public:
    explicit QInfoBox(QWidget* parent = NULL);
    ~QInfoBox();

 signals:
    void signalConfirm();
public slots:
  void slotConfirm();
      
 public:
    QPushButton *btnConfirm;
    QPushButton *btnCancel;

    QLabel *labelInfo1;
    QLabel *labelInfo2;
    QWidget *m_pWidget;
    void SetShowInfo(QString strInfo);
};

#endif // QLOGIN_H
