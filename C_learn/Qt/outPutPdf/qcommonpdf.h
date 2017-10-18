#ifndef QCOMMONPDF_H
#define QCOMMONPDF_H

#include <QWidget>
#include <QPrintDialog>
#include <QPrinter>
#include <QTextEdit>
#include <QLabel>
#include <QPainter>
#include <QDateTime>
#include <QColor>
#include <QStyle>
#include <QPicture>

#define TEST_PDFNAME "test.pdf"


#define ARM_A8 1

#ifdef ARM_A8

#define A4WDH 1.4145  //A4纸长宽比
#define A4_WIDTH  10000
#define A4_HEIGHT 14145



#define PDF_PSIZE_HEADLINE  10
#define PDF_PSIZE_TITLE  5
#define PDF_PSIZE_STR  1
#define PDF_PSIZE_LINE 4

#define PDF_PixSIZE_HEADLINE  160
#define PDF_PixSIZE_TITLE  140
#define PDF_PixSIZE_STR  150

#define DEFAULT_WIDTH 1500
#define DEFAULT_HEIGHT 240



#else

#define A4_WIDTH  10000
#define A4_HEIGHT 13800

#endif

#define PDF_TABLE_CNT_MAX 20
#define PDF_COLOR_TITLER QColor(200,200,200)
#define PDF_COLOR_LINE  QColor(30,60,30)




typedef struct{
  QPoint pos1;
  QPoint pos2;
  int pointsize;
  QColor color;
}NLine;

typedef struct{
  QRect rect;
  QColor color;
}NRect;

typedef struct {
  QRect rect;
  QFont font;
  QColor color;
  QString str;
  int Psize;
}NString;

typedef struct {
  QRect rect;
  QPixmap pixmap;
}NPixmap;

typedef struct {
  QRect rect;
  QPicture picture;
}NPicture;

typedef struct{
  QPoint pos;
  int table_w;
  int table_h;

  int row;
  int col;
  int Height[PDF_TABLE_CNT_MAX];
  int Width[PDF_TABLE_CNT_MAX];

  QString headline;
  int headPsize;
  int headW;
  int headH;
  int headFontPixsize;
  QColor headColor;


  QString strTitle[PDF_TABLE_CNT_MAX];
  int titlePsize;
  QColor titleColor;
  int titleFontPixsize;
  QColor titleRectColor;

  int strPsize;
  int strFontPixsize;
  QColor TextColor;

  int linePsize;
  QColor lineColor;


  int curRowHeight;
  int curRow;
  int curColWidth;
  int curCol;
}NTableDes;

typedef struct{
  QList<NString*> *pNStr;
  QList<NPixmap*> *pNPix;
  QList<NLine*>   *pNLin;
  QList<NRect*>   *pNRec;
  QList<NPicture*> *pNPic;
}PdfPageData;

class QCommonPdf : public QObject
{
    Q_OBJECT
public:
    explicit QCommonPdf(QObject *parent = 0);
    ~QCommonPdf();

    void NewPdf(QString strName);
    void CreatePageNow();
    void NewPdfPage();
    void EndPdf();
//Table
NTableDes *PrepareTable(QPoint pos , int row, int col, QString headline, QString strT[]);

void SetRowHeight(NTableDes *tableDes , int index , int Height){tableDes->Height[index] = Height;}
void SetColWidth(NTableDes *tableDes , int index , int Width){tableDes->Width[index] = Width;}


void SetHeadLineRSize(NTableDes *tableDes , int w , int h){tableDes->headW = w;tableDes->headH = h;}
void SetHeadLinePSize(NTableDes *tableDes , int psize){tableDes->headPsize = psize;}
void SetHeadLineColor(NTableDes *tableDes , QColor color){tableDes->headColor = color;}

void SetTitleRColor(NTableDes *tableDes , QColor color){tableDes->titleRectColor = color;}
void SetTitlePSize(NTableDes *tableDes , int psize){tableDes->titlePsize = psize;}
void SetTitleColor(NTableDes *tableDes , QColor color){tableDes->titleColor = color;}
void SetTitleFontWidth(NTableDes *tableDes , int pixsize){tableDes->titleFontPixsize = pixsize;}

void SetTextColor(NTableDes *tableDes ,QColor color){tableDes->TextColor = color;}
void SetTextPSize(NTableDes *tableDes ,int psize){tableDes->strPsize = psize;}

void SetLineColor(NTableDes *tableDes ,QColor color){tableDes->lineColor = color;}
void SetLinePSize(NTableDes *tableDes ,int psize){tableDes->linePsize = psize;}

void CreateTable(NTableDes *tableDes);
void InsertRow(NTableDes *tableDes , QString str[]);
void InsertRow(NTableDes *tableDes , QString str[] , int index , QColor color);
// 使用了 InserRow 就不要使用 InserCol
void InsertCol(NTableDes *tableDes , QString str[]);



void ExportLine(QPoint pos1 , QPoint pos2 , int pointsize , QColor color = Qt::black);
void ExportRect(QPoint leftu ,QPoint rightb , QColor color);

void ExportString(QRect rect, QString str, int psize = 15, QColor color = Qt::black);
void ExportString(QRect rect, QString str, QFont font , int psize = 15, QColor color = Qt::black);

void ExportImage(QRect rect , QPixmap pixmap);
void ExportImage(QRect rect,  QPicture picture);
void ExportWidget(QRect rect , QWidget *widget);



void PaintPdfPage(PdfPageData* pdfPage);
void ClearPaintData(PdfPageData* pdfPage);

private:
    QPrinter *_printer;
    QPainter *_painter;
    QList<NString*> *list_NStr;
    QList<NPixmap*> *list_NPix;
    QList<NPicture*> *list_NPic;
    QList<NLine*>  *list_NLin;
    QList<NRect*>  *list_NRec;

    PdfPageData* CurPage;
    QList<NTableDes*> list_NTable;
    QList<PdfPageData*> listPage;

    QFont _font;
    QPen _pen;
signals:

public slots:


};

#endif // QCOMMONPDF_H
