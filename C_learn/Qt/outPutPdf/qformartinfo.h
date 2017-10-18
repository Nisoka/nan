#ifndef QFORMARTINFO_H
#define QFORMARTINFO_H

#include <QObject>
#include "qcommonpdf.h"
#include "typedefine.h"

#define PDF_FORMART 1

#ifdef ENABLE_TEC_FUNC

#define TEST_INTO_ROW 11
#define TEST_INFO_COL 2

#define TEST_COND_ROW 5
#define TEST_COND_COL 2

#define TEST_RESU_ROW 6
#define TEST_RESU_COL 2

#define TEST_EVENT_ROW_MAX 12
#define TEST_EVENT_COL 8



#define OTDR_HEAD_X 0
#define OTDR_HEAD_Y -600
#define OTDR_HEAD_W 9000
#define OTDR_HEAD_H 1500
//postions
#define TEST_INFO_X 0000
#define TEST_INFO_Y 1000
#define TEST_INFO_W 4000
//#define TEST_INTO_H 6000

#define TEST_COND_X 5000
#define TEST_COND_Y 1000
#define TEST_COND_W TEST_INTO_W
//#define TEST_COND_H 6000

#define TEST_RESU_X 5000
#define TEST_RESU_Y 2500
#define TEST_RESU_W TEST_INTO_W
//#define TEST_COND_H 6000

#define RESU_PIX_X 0000
#define RESU_PIX_Y 5000
#define RESU_PIX_W 8000
#define RESU_PIX_H 4300

#define FIBERSPOT_PIX_X 600
#define FIBERSPOT_PIX_Y 2000
#define FIBERSPOT_PIX_W 8000
#define FIBERSPOT_PIX_H 5000

#define TEST_EVENT_X 0000
#define TEST_EVENT_Y 9400

#else

#define TEST_INTO_ROW 14
#define TEST_INFO_COL 2

#define TEST_COND_ROW 5
#define TEST_COND_COL 2

#define TEST_RESU_ROW 5
#define TEST_RESU_COL 2

#define TEST_EVENT_ROW_MAX 12
#define TEST_EVENT_COL 8



#define OTDR_HEAD_X 0
#define OTDR_HEAD_Y -600
#define OTDR_HEAD_W 9000
#define OTDR_HEAD_H 1500
//postions
#define TEST_INFO_X 0000
#define TEST_INFO_Y 1000
#define TEST_INFO_W 4000
//#define TEST_INTO_H 6000

#define TEST_COND_X 5000
#define TEST_COND_Y 1000
#define TEST_COND_W TEST_INTO_W
//#define TEST_COND_H 6000

#define TEST_RESU_X 5000
#define TEST_RESU_Y 3000
#define TEST_RESU_W TEST_INTO_W
//#define TEST_COND_H 6000

#define RESU_PIX_X 0000
#define RESU_PIX_Y 5000
#define RESU_PIX_W 8000
#define RESU_PIX_H 4300

#define FIBERSPOT_PIX_X 600
#define FIBERSPOT_PIX_Y 2000
#define FIBERSPOT_PIX_W 8000
#define FIBERSPOT_PIX_H 5000

#define TEST_EVENT_X 0000
#define TEST_EVENT_Y 9400

#endif
//#define TEST_EVENT_W
//#define TEST_COND_H 6000




//#define EVENTWELD 0
//#define EVENTSPLICE 2
//#define EVENTSTREAK 3

//#define EVENTCHAIN 1
//#define EVENTSLOPE 7

//#define EVENTGAIN 4      //USER ADD

//#define EVENTHEAD 6
//#define EVENTEND 5


#define EVENT_SPLICE      0
#define EVENT_REFLECTION  1
#define EVENT_SPLICE_ANI  2
#define EVENT_STREAKING   3

#define EVENT_END         9

#define BADVALUE 1234.5678
#define BADVALUE_INT 55554

typedef struct Event_Detail_Data
{
    int ID;
    int Type;
    double Dis;
    double Loss;
    double Refl;
    double Slp;
    double Sec;
    double TLos;

    bool SelectFlag;
    int FailReason;
 } Event_Detail;


typedef struct {
    QCommonPdf* pdf;
}PdfDesCr;


class QOTDRResultPdf : public QObject
{
    Q_OBJECT
public:
    explicit QOTDRResultPdf(QObject *parent = 0);

    void InitOutpInfo(QString Max_path , int index);

    void NewPdf(QString strFileName);
    void PainterPage();
    void EndPdf();

    void OutpTestInfoData(QString TestInfoData[]);
    void OutpTestCondData(QString TestCondData[]);
    void OutpTestResuData(QString TestResuData[]);
    void OutpTestResuPic(QPicture picture);
    void OutpFiberSpot(QPixmap picture);

    void EventHeadDetails(QString str[]);
    void OutpTestEventsData(QString TestEventsData[][TEST_EVENT_COL] , int Rows);

private:
    QCommonPdf *commonPdf;

signals:

public slots:
};

#endif // QFORMARTINFO_H
