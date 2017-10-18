#include "qcommonpdf.h"

QCommonPdf::QCommonPdf(QObject *parent) : QObject(parent)
    ,_printer(NULL)
{

    _font.setPointSize(10);

    _painter = new QPainter;


    //永远只有一页！
    list_NStr = new QList<NString*>;
    list_NLin = new QList<NLine*>;
    list_NRec = new QList<NRect*>;
    list_NPix = new QList<NPixmap*>;
    list_NPic = new QList<NPicture*>;

    CurPage = new PdfPageData;
    CurPage->pNStr = list_NStr;
    CurPage->pNLin = list_NLin;
    CurPage->pNRec = list_NRec;
    CurPage->pNPix = list_NPix;
    CurPage->pNPic = list_NPic;
    listPage.append(CurPage);
}

QCommonPdf::~QCommonPdf()
{
}


void QCommonPdf::NewPdf(QString strName)
{
    _printer = new QPrinter(QPrinter::HighResolution);
    _printer->setPageSize(QPrinter::A4);  //设置纸张大小为A4
    _printer->setOutputFormat(QPrinter::PdfFormat);  //设置输出格式为pdf
    _printer->setOutputFileName(strName);

    _painter->begin(_printer);
    _painter->scale(1,1);
}


void QCommonPdf::CreatePageNow()
{

    PdfPageData* pdfPage = NULL;
    NTableDes *table = NULL;

    pdfPage = listPage.at(0);
    listPage.removeFirst();
    PaintPdfPage(pdfPage);


    while(list_NTable.count() > 0)
    {
        table = list_NTable.at(0);
        list_NTable.removeFirst();
        if(table != NULL)
            delete table;
    }

}

void QCommonPdf::NewPdfPage()
{
    _printer->newPage();
    //可提升速度，多页一起打印，这时候需要修正
    listPage.append(CurPage);
}


void QCommonPdf::EndPdf()
{
   _painter->end();
   if(_printer != NULL)
    {
       delete _printer;
       _printer = NULL;
   }
}



void QCommonPdf::ExportRect(QPoint leftu ,QPoint rightb , QColor color)
{
    NRect *Nrec = new NRect;
    Nrec->rect = QRect(leftu , rightb);
    Nrec->color = color;
    list_NRec->append(Nrec);
}


void QCommonPdf::ExportLine(QPoint pos1 , QPoint pos2 , int pointsize, QColor color)
{
    NLine *Nline = new NLine;
    Nline->pos1 = pos1;
    Nline->pos2 = pos2;
    Nline->pointsize = pointsize;
    Nline->color = color;
    list_NLin->append(Nline);
}



void QCommonPdf::ExportString(QRect rect, QString str, QFont font , int psize, QColor color)
{
    NString *Nstr = new NString;
    Nstr->rect = rect;
    Nstr->font = font;
    Nstr->color = color;
    Nstr->Psize = psize;
    Nstr->str = str;


    list_NStr->append(Nstr);
}

void QCommonPdf::ExportString(QRect rect, QString str, int psize, QColor color)
{
    ExportString(rect , str , _font , psize , color);
}



void QCommonPdf::ExportImage(QRect rect, QPixmap pixmap)
{
    NPixmap *Npix = new NPixmap;
    Npix->rect = rect;
    Npix->pixmap = pixmap;

    printf("In the ExportImage \r\n");

    list_NPix->append(Npix);
}


void QCommonPdf::ExportImage(QRect rect, QPicture picture)
{
    NPicture *Npix = new NPicture;
    Npix->rect = rect;
    Npix->picture = picture;

    printf("In the ExportImage \r\n");

    list_NPic->append(Npix);
}



void QCommonPdf::ExportWidget(QRect rect , QWidget *widget)
{
    NPixmap *Npix = new NPixmap;
    Npix->rect = rect;
    Npix->pixmap = QPixmap::grabWidget(widget, widget->rect());  //获取界面的图片

    list_NPix->append(Npix);
}



/**
 * @brief QCommonPdf::PrepareTable
 * @param pos
 * @param row
 * @param col
 * @param headline
 * @param strTitle
 * @return
 */

NTableDes* QCommonPdf::PrepareTable(QPoint pos , int row , int col  , QString headline , QString strTitle[])
{
    int i = 0;
    NTableDes *tableDes = new NTableDes;

    tableDes->pos = pos;
    tableDes->row = row;
    tableDes->col = col;
    tableDes->table_w = 0;
    tableDes->table_h = 0;

    tableDes->headPsize = PDF_PSIZE_HEADLINE;
    tableDes->headFontPixsize = PDF_PixSIZE_HEADLINE;
    tableDes->headColor = Qt::black;
    tableDes->headH = DEFAULT_HEIGHT+80;
    tableDes->headW = DEFAULT_WIDTH+200;

    tableDes->titlePsize = PDF_PSIZE_TITLE;
    tableDes->titleRectColor = PDF_COLOR_TITLER;
    tableDes->titleFontPixsize = PDF_PixSIZE_TITLE;
    tableDes->titleColor = Qt::black;

    tableDes->strPsize = PDF_PSIZE_STR;
    tableDes->strFontPixsize = PDF_PixSIZE_STR;
    tableDes->TextColor = Qt::black;

    tableDes->linePsize = PDF_PSIZE_LINE;
    tableDes->lineColor = PDF_COLOR_LINE;

    tableDes->curRowHeight = 0;
    tableDes->curRow = 0;
    tableDes->curColWidth = 0;
    tableDes->curCol = 0;


    tableDes->headline = headline;

    for(i = 0 ; i < row ; i++)
    {
        tableDes->Height[i] = DEFAULT_HEIGHT;
    }

    for(i = 0 ; i < col ; i++)
    {
        tableDes->strTitle[i] = strTitle[i];
        tableDes->Width[i] = DEFAULT_WIDTH;
    }

    list_NTable.append(tableDes);
    return tableDes;
}


void QCommonPdf::CreateTable(NTableDes *tableDes)
{


    NLine* line = NULL;
    NString* str = NULL;
    int i = 0;
    int Width = 0;
    int Height = 0;
    NRect *Nrec = NULL;
    char buf[100];

    for( i = 0 ; i < tableDes->col ; i++ ){
        tableDes->table_w += tableDes->Width[i];
    }

    for( i = 0 ; i < tableDes->row ; i++ ){
        tableDes->table_h += tableDes->Height[i];
    }



    memset(buf , 0 , 100);

    //Headline
    str = new NString;
    str->str = tableDes->headline;
    str->rect = QRect(QPoint(tableDes->pos.x() , tableDes->pos.y()-tableDes->headH),
                      QSize(tableDes->headW , tableDes->headH));
    str->Psize = tableDes->headPsize;
    str->font.setBold(true);
    str->font.setPixelSize(tableDes->headFontPixsize);
    str->color = tableDes->headColor;
    list_NStr->append(str);

    //Row Lines
    for( i = 0 ; i < tableDes->row + 1; i++ ){
        line = new NLine;
        line->pointsize = tableDes->linePsize;
        line->color = tableDes->lineColor;
        line->pos1 = QPoint(tableDes->pos.x() ,
                            tableDes->pos.y() + Height);
        line->pos2 = QPoint(tableDes->pos.x() + tableDes->table_w,
                            tableDes->pos.y() + Height);

        list_NLin->append(line);

        Height += tableDes->Height[i];

        printf("Height = %d \r\n" , Height);
    }

    //Col Lines  and Title Rect  and Title
    for(i = 0 ; i < tableDes->col + 1 ; i++)
    {
        line = new NLine;
        line->pointsize = tableDes->linePsize;
        line->color = tableDes->lineColor;
        line->pos1 = QPoint(tableDes->pos.x() + Width,
                            tableDes->pos.y());

        line->pos2 = QPoint(tableDes->pos.x() + Width,
                            tableDes->pos.y() + tableDes->table_h);

        list_NLin->append(line);

        Width += tableDes->Width[i];

        //Titles
        if(i < tableDes->col){
            str = new NString;
            str->str = tableDes->strTitle[i];
            str->color = tableDes->titleColor;
            str->Psize = tableDes->titlePsize;
            str->font.setPixelSize(tableDes->titleFontPixsize);
            str->font.setBold(true);
            str->rect = QRect(line->pos1.x() , line->pos1.y() ,
                                   tableDes->Width[i], tableDes->Height[0]);
            list_NStr->append(str);

            //Title Rect;
            if(i == 0){
                Nrec = new NRect;
                Nrec->color = tableDes->titleRectColor;
                Nrec->rect = QRect(line->pos1.x() , line->pos1.y() ,
                                   tableDes->table_w , tableDes->Height[0]);
                printf("Title Rect x=%d  y=%d W=%d h=%d" , line->pos1.x() , line->pos1.y() ,
                       tableDes->table_w , tableDes->Height[0]);
                list_NRec->append(Nrec);

                //表头高度增加，准备以后inserRow使用
                tableDes->curRowHeight += tableDes->Height[tableDes->curRow];
                tableDes->curRow +=1;
            }
        }
    }
}

void QCommonPdf::InsertRow(NTableDes *tableDes , QString str[] , int index , QColor color)
{
    NString *Nstr = NULL;
    QPoint pos;
    int Width = 0;
    int i = 0;

    for(i = 0 ; i < tableDes->col; i++){
        Nstr = new NString;
        Nstr->str = str[i];
        Nstr->Psize = tableDes->strPsize;
        pos.setX(tableDes->pos.x()+Width);
        pos.setY(tableDes->pos.y()+tableDes->curRowHeight);
        Width += tableDes->Width[i];

        Nstr->rect = QRect(pos,QSize(tableDes->Width[i], tableDes->Height[tableDes->curRow]));
        if(i != index)
            Nstr->color = tableDes->TextColor;
        else
            Nstr->color = color;
        list_NStr->append(Nstr);
    }

    tableDes->curRowHeight += tableDes->Height[tableDes->curRow];
    tableDes->curRow +=1;
}

void QCommonPdf::InsertRow(NTableDes *tableDes , QString str[])
{
    NString *Nstr = NULL;
    QPoint pos;
    int Width = 0;
    int i = 0;

    for(i = 0 ; i < tableDes->col ; i++){
        Nstr = new NString;
        Nstr->str = str[i];
        Nstr->Psize = tableDes->strPsize;
        Nstr->font.setPixelSize(tableDes->strFontPixsize);
        pos.setX(tableDes->pos.x()+Width);
        pos.setY(tableDes->pos.y()+tableDes->curRowHeight);
        Width += tableDes->Width[i];

        Nstr->rect = QRect(pos,QSize(tableDes->Width[i], tableDes->Height[tableDes->curRow]));
        Nstr->color = tableDes->TextColor;
        list_NStr->append(Nstr);
    }

    tableDes->curRowHeight += tableDes->Height[tableDes->curRow];
    tableDes->curRow +=1;
}


void QCommonPdf::InsertCol(NTableDes *tableDes , QString str[])
{
    NString *Nstr = NULL;
    QPoint pos;
    int Height = 0;
    int i = 0;

    //i=1 不插入表头列
    for(i = 1 ; i < tableDes->row+1 ; i++){
        Nstr = new NString;
        Nstr->str = str[i];
        Nstr->Psize = tableDes->strPsize;
        Height += tableDes->Height[i-1];
        pos.setX(tableDes->pos.x()+Height);
        pos.setY(tableDes->pos.y()+tableDes->curColWidth);

        Nstr->rect = QRect(pos,QSize(tableDes->Width[tableDes->curCol], tableDes->Height[i]));
        Nstr->color = tableDes->TextColor;
        list_NStr->append(Nstr);
    }

    tableDes->curColWidth += tableDes->Height[tableDes->curRow];
    tableDes->curCol +=1;
}






void QCommonPdf::ClearPaintData(PdfPageData* pdfPage)
{

    QList<NString*> *iNStr = pdfPage->pNStr;
    QList<NPixmap*> *iNPix = pdfPage->pNPix;
    QList<NPicture*> *iNPic = pdfPage->pNPic;
    QList<NLine*>   *iNLin = pdfPage->pNLin;
    QList<NRect*>   *iNRec = pdfPage->pNRec;

    NString *Nstr = NULL;
    while(iNStr->count() > 0)
    {
        Nstr = iNStr->at(0);
        iNStr->removeFirst();
        if(Nstr != NULL)
            delete Nstr;
    }

    NPixmap *Npix = NULL;
    while(iNPix->count() > 0)
    {
        Npix = iNPix->at(0);
        iNPix->removeFirst();
        if(Npix != NULL)
            delete Npix;
    }
    NPicture *Npic = NULL;
    while(iNPic->count() > 0)
    {
        Npic = iNPic->at(0);
        iNPic->removeFirst();
        if(Npic != NULL)
            delete Npic;
    }

    NLine *line = NULL;
    while(iNLin->count() > 0)
    {
        line = iNLin->at(0);
        iNLin->removeFirst();
        if(line != NULL)
            delete line;
    }

    NRect *rec = NULL;
    while(iNRec->count() > 0)
    {
        rec = iNRec->at(0);
        iNRec->removeFirst();
        if(rec != NULL)
            delete rec;
    }
}

void QCommonPdf::PaintPdfPage(PdfPageData* pdfPage)
{

    QList<NString*> *iNStr = pdfPage->pNStr;
    QList<NPixmap*> *iNPix = pdfPage->pNPix;
    QList<NLine*>   *iNLin = pdfPage->pNLin;
    QList<NRect*>   *iNRec = pdfPage->pNRec;
    QList<NPicture*> *iNPic = pdfPage->pNPic;

    _painter->setPen(_pen);
    _painter->setFont(_font);
    int i = 0;
    QPen pen;
    QBrush brush;

    NRect *Nrec = NULL;
    brush.setStyle(Qt::SolidPattern);
    for(i = 0 ; i < iNRec->count() ; i++)
    {
        Nrec = iNRec->at(i);
        brush.setColor(Nrec->color);
        _painter->setBrush(brush);
        _painter->drawRect(Nrec->rect);
    }
    _painter->setBrush(Qt::NoBrush);


    NLine *Nlin = NULL;
    for(i = 0 ; i < iNLin->count() ; i++)
    {
        Nlin = iNLin->at(i);
        pen.setWidth(Nlin->pointsize);
        pen.setColor(Nlin->color);
        _painter->setPen(pen);
        _painter->drawLine(Nlin->pos1 , Nlin->pos2);
    }
    _painter->setPen(_pen);


    NString *Nstr = NULL;
    for( i = 0 ; i < iNStr->count() ; i++)
    {
        Nstr = iNStr->at(i);
        pen.setWidth(Nstr->Psize);
        pen.setColor(Nstr->color);
        _painter->setPen(pen);
        _painter->setFont(Nstr->font);
        _painter->drawText(Nstr->rect , Qt::AlignCenter , Nstr->str);
    }
    _painter->setPen(_pen);
    _painter->setFont(_font);


    NPixmap *Npix = NULL;
    for( i = 0 ; i < iNPix->count() ; i++)
    {
        Npix = iNPix->at(i);
        _painter->drawPixmap(Npix->rect, Npix->pixmap);
    }


    NPicture *Npic = NULL;
    for( i = 0 ; i < iNPic->count() ; i++)
    {
        Npic = iNPic->at(i);


//        _painter->scale(0.8,0.8);
//        _painter->drawPicture(QPoint(Npic->rect.x()*10/8 , Npic->rect.y()*10/8)
//                              , Npic->picture);
//        _painter->scale(1,1);
        _painter->drawPicture(QPoint(Npic->rect.x() , Npic->rect.y())
                              , Npic->picture);

    }



    ClearPaintData(pdfPage);
}




