#include "qformartinfo.h"

static char trTextpdf[200][256];

#define English                         0
#define Chinese                         1
#define French                          2
#define Spanish                         3
#define Portuguese                      4
#define Russian                         5
#define Italian                         6
#define German                          7
#define ChineseT                        8       //Chinese Traditional  : CHT
#define Japanese                        9       //Japanese: JPN
#define Korean                          10      //Korean:   KOR



#ifdef ENABLE_TEC_FUNC
#define LANGUAGE_ENG_PATH   			"/showlanguage/OutpInfo/TEC/LanguageEng.tra"
#define LANGUAGE_CHI_PATH   			"/showlanguage/OutpInfo/TEC/LanguageChi.tra"
#define LANGUAGE_FRE_PATH   			"/showlanguage/OutpInfo/TEC/LanguageFre.tra"
#define LANGUAGE_SPA_PATH   			"/showlanguage/OutpInfo/TEC/LanguageSpa.tra"
#define LANGUAGE_POR_PATH   			"/showlanguage/OutpInfo/TEC/LanguagePor.tra"
#define LANGUAGE_RUS_PATH   			"/showlanguage/OutpInfo/TEC/LanguageRus.tra"
#define LANGUAGE_ITA_PATH   			"/showlanguage/OutpInfo/TEC/LanguageIta.tra"
#define LANGUAGE_GER_PATH               "/showlanguage/OutpInfo/TEC/LanguageGer.tra"
#define LANGUAGE_CNT_PATH               "/showlanguage/OutpInfo/TEC/LanguageCnt.tra"
#define LANGUAGE_JPN_PATH               "/showlanguage/OutpInfo/TEC/LanguageJpn.tra"
#define LANGUAGE_KOR_PATH               "/showlanguage/OutpInfo/TEC/LanguageKor.tra"
#else
#define LANGUAGE_ENG_PATH   			"/showlanguage/OutpInfo/LanguageEng.tra"
#define LANGUAGE_CHI_PATH   			"/showlanguage/OutpInfo/LanguageChi.tra"
#define LANGUAGE_FRE_PATH   			"/showlanguage/OutpInfo/LanguageFre.tra"
#define LANGUAGE_SPA_PATH   			"/showlanguage/OutpInfo/LanguageSpa.tra"
#define LANGUAGE_POR_PATH   			"/showlanguage/OutpInfo/LanguagePor.tra"
#define LANGUAGE_RUS_PATH   			"/showlanguage/OutpInfo/LanguageRus.tra"
#define LANGUAGE_ITA_PATH   			"/showlanguage/OutpInfo/LanguageIta.tra"
#define LANGUAGE_GER_PATH               "/showlanguage/OutpInfo/LanguageGer.tra"
#define LANGUAGE_CNT_PATH               "/showlanguage/OutpInfo/LanguageCnt.tra"
#define LANGUAGE_JPN_PATH               "/showlanguage/OutpInfo/LanguageJpn.tra"
#define LANGUAGE_KOR_PATH               "/showlanguage/OutpInfo/LanguageKor.tra"
#endif

static int LoadTranslateText2( QString dir, char text[][256])
{
    FILE* pFile = NULL;
    int counter = 1;

    char *str;

    QByteArray ba = dir.toLatin1();

    str=ba.data();


    if((pFile = fopen(str, "r")) != NULL)
    {

        while(fgets(text[counter], sizeof(text[counter]), pFile) != NULL)
        {
#ifndef QT_CREATOR_DEBUG
            text[counter][strlen(text[counter]) - 2] = 0;
#else
         // text[counter][strlen(text[counter]) - 1] = 0;// 原来是 text[counter]) - 2
#endif
            printf("%s\r\n" , text[counter]);
            counter++;

        }
        fclose(pFile);
        system("sync");
        return 0;

    }
    else
    {

        return -1;
    }
}

static int SetCurrentTranslateText2(QString ParentPath , int LanguageIndex)
{
    memset(trTextpdf, 0, sizeof(trTextpdf));
    if(LanguageIndex == Chinese)
    {
        if(LoadTranslateText2(ParentPath +LANGUAGE_CHI_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath +LANGUAGE_CHI_PATH, trTextpdf);
        }
    }

    else if(LanguageIndex == French)
    {
        if(LoadTranslateText2(ParentPath +LANGUAGE_FRE_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_FRE_PATH, trTextpdf);
        }

    }
    else if(LanguageIndex == Spanish)
    {
        if(LoadTranslateText2(ParentPath +LANGUAGE_SPA_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath +LANGUAGE_SPA_PATH, trTextpdf);
        }

    }
    else if(LanguageIndex == Portuguese)
    {
        if(LoadTranslateText2(ParentPath + LANGUAGE_POR_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_POR_PATH, trTextpdf);
        }

    }
    else if(LanguageIndex == Russian)
    {
        if(LoadTranslateText2(ParentPath + LANGUAGE_RUS_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_RUS_PATH, trTextpdf);
        }

    }
    else if(LanguageIndex == Italian)
    {
        if(LoadTranslateText2(ParentPath + LANGUAGE_ITA_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_ITA_PATH, trTextpdf);
        }

    }
    else if(LanguageIndex == German)
    {
        if(LoadTranslateText2(ParentPath + LANGUAGE_GER_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_GER_PATH, trTextpdf);
        }
    }
    else if(LanguageIndex == ChineseT)
    {
        printf(" == ChineseT !\n");
        if(LoadTranslateText2(ParentPath + LANGUAGE_CNT_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_CNT_PATH, trTextpdf);
            printf(" Load ChineseT OK!\n");
        }
    }
    else if(LanguageIndex == Japanese)
    {
        if(LoadTranslateText2(ParentPath + LANGUAGE_JPN_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_JPN_PATH, trTextpdf);
        }
    }
    else if(LanguageIndex == Korean)
    {
        if(LoadTranslateText2(ParentPath + LANGUAGE_KOR_PATH, trTextpdf) == -1)
        {
            LoadTranslateText2(ParentPath + LANGUAGE_KOR_PATH, trTextpdf);
        }
    }
    else
    {
        LoadTranslateText2(ParentPath +LANGUAGE_ENG_PATH, trTextpdf);
    }

    return 0;
}








QOTDRResultPdf::QOTDRResultPdf(QObject *parent) : QObject(parent)
{
    commonPdf = new QCommonPdf;
}


void QOTDRResultPdf::InitOutpInfo(QString Max_path , int index)
{
    SetCurrentTranslateText2(Max_path , index);
}

void QOTDRResultPdf::NewPdf(QString strFileName)
{
    commonPdf->NewPdf(strFileName);
}

void QOTDRResultPdf::PainterPage()
{
    QFont fnt;
    fnt.setPixelSize(300);
    commonPdf->ExportString(QRect(OTDR_HEAD_X , OTDR_HEAD_Y,OTDR_HEAD_W,OTDR_HEAD_H),trTextpdf[64],fnt);
    commonPdf->ExportString(QRect(1500 , 13000 , 10000 , 300) , trTextpdf[60]);
    commonPdf->CreatePageNow();
}

void QOTDRResultPdf::EndPdf()
{
    commonPdf->EndPdf();
}


void QOTDRResultPdf::OutpTestInfoData(QString TestInfoData[])
{
    QString strData[TEST_INFO_COL];
    int i = 0;

    strData[0] = trTextpdf[2];
    strData[1] = trTextpdf[3];


    NTableDes *tableInfo = commonPdf->PrepareTable(
                QPoint(TEST_INFO_X , TEST_INFO_Y), TEST_INTO_ROW , TEST_INFO_COL , trTextpdf[1], strData);
    commonPdf->SetColWidth(tableInfo , 1 , 2800);
    commonPdf->CreateTable(tableInfo);

    //Title 不进入计算
    for(i = 1 ; i < TEST_INTO_ROW ; i++)
    {
        strData[0] = trTextpdf[3+i];
        strData[1] = TestInfoData[i-1];
        commonPdf->InsertRow(tableInfo , strData);
    }
}

void QOTDRResultPdf::OutpTestCondData(QString TestCondData[])
{
    QString strData[TEST_COND_COL];
    int i = 0;

    strData[0] = trTextpdf[22];
    strData[1] = trTextpdf[23];

    NTableDes *tableInfo = commonPdf->PrepareTable(
                QPoint(TEST_COND_X , TEST_COND_Y), TEST_COND_ROW , TEST_COND_COL , trTextpdf[21], strData);
    commonPdf->CreateTable(tableInfo);

    //Title 不进入计算
    for(i = 1 ; i < TEST_COND_ROW ; i++)
    {
        strData[0] = trTextpdf[23+i];
        strData[1] = TestCondData[i-1];
        commonPdf->InsertRow(tableInfo , strData);
    }
}

void QOTDRResultPdf::OutpTestResuData(QString TestResuData[])
{
    QString strData[TEST_RESU_COL];
    int i = 0;

    strData[0] = trTextpdf[32];
    strData[1] = trTextpdf[33];

    NTableDes *tableInfo = commonPdf->PrepareTable(
                QPoint(TEST_RESU_X , TEST_RESU_Y), TEST_RESU_ROW , TEST_RESU_COL , trTextpdf[31], strData);
    commonPdf->CreateTable(tableInfo);

    //Title 不进入计算
    for(i = 1 ; i < TEST_RESU_ROW ; i++)
    {
        strData[0] = trTextpdf[33+i];
        strData[1] = TestResuData[i-1];
        commonPdf->InsertRow(tableInfo , strData);
    }
}

void QOTDRResultPdf::OutpTestResuPic(QPicture picture)
{
    printf("In the OutpTestResuPix \r\n");
    //commonPdf->ExportImage(QRect(RESU_PIX_X,RESU_PIX_Y,RESU_PIX_W,RESU_PIX_H) , pixmap);
    commonPdf->ExportImage(QRect(RESU_PIX_X,RESU_PIX_Y,RESU_PIX_W,RESU_PIX_H) , picture);
}

void QOTDRResultPdf::OutpFiberSpot(QPixmap picture)
{
    PainterPage();
    commonPdf->NewPdfPage();
    commonPdf->ExportImage(QRect(FIBERSPOT_PIX_X,FIBERSPOT_PIX_Y,FIBERSPOT_PIX_W,FIBERSPOT_PIX_H) , picture);
}


void QOTDRResultPdf::EventHeadDetails(QString str[])
{
        sprintf(trTextpdf[42] , str[0].toStdString().c_str());
        sprintf(trTextpdf[43] , str[1].toStdString().c_str());
        sprintf(trTextpdf[44] , str[2].toStdString().c_str());
        sprintf(trTextpdf[45] , str[3].toStdString().c_str());
        sprintf(trTextpdf[46] , str[4].toStdString().c_str());
        sprintf(trTextpdf[47] , str[5].toStdString().c_str());
        sprintf(trTextpdf[48] , str[6].toStdString().c_str());
        sprintf(trTextpdf[49] , str[7].toStdString().c_str());
}

void QOTDRResultPdf::OutpTestEventsData(QString TestEventsData[][TEST_EVENT_COL] , int Rows)
{
    QString strData[TEST_EVENT_COL];
    int ColWidth[TEST_EVENT_COL];

    int i = 0;
    int j = 0;
    int temp = 0;
    NTableDes *tableInfo = NULL;
    static int Pos_x = TEST_EVENT_X;
    static int Pos_y = TEST_EVENT_Y;

    ColWidth[0] = DEFAULT_WIDTH/3;
    ColWidth[1] = DEFAULT_WIDTH/3;
    ColWidth[2] = DEFAULT_WIDTH*3/4;
    ColWidth[3] = DEFAULT_WIDTH*3/4;
    ColWidth[4] = DEFAULT_WIDTH*3/4;
    ColWidth[5] = DEFAULT_WIDTH;
    ColWidth[6] = DEFAULT_WIDTH*2/3;
    ColWidth[7] = DEFAULT_WIDTH;




    if(Rows > TEST_EVENT_ROW_MAX)
    {

        strData[0] = trTextpdf[42];
        strData[1] = trTextpdf[43];
        strData[2] = trTextpdf[44];
        strData[3] = trTextpdf[45];
        strData[4] = trTextpdf[46];
        strData[5] = trTextpdf[47];
        strData[6] = trTextpdf[48];
        strData[7] = trTextpdf[49];

        tableInfo = commonPdf->PrepareTable(
                    QPoint(TEST_EVENT_X , TEST_EVENT_Y), TEST_EVENT_ROW_MAX , TEST_EVENT_COL , trTextpdf[21], strData);
        for(i = 0 ; i < TEST_EVENT_COL ; i++)
        {
            commonPdf->SetColWidth(tableInfo , i , ColWidth[i]);
        }
        commonPdf->CreateTable(tableInfo);

        //Title 不进入计算
        for(i = 1 ; i < TEST_EVENT_ROW_MAX ; i++)
        {
            for(j = 0 ; j < TEST_EVENT_COL ; j++)
            {
                strData[j] = TestEventsData[i-1][j];
            }
            commonPdf->InsertRow(tableInfo , strData);
        }

        Pos_x = 0000;
        Pos_y = TEST_INFO_Y;

        temp = Rows - TEST_EVENT_ROW_MAX + 1;
        Rows = temp;

        PainterPage();
        commonPdf->NewPdfPage();
    }

    strData[0] = trTextpdf[42];
    strData[1] = trTextpdf[43];
    strData[2] = trTextpdf[44];
    strData[3] = trTextpdf[45];
    strData[4] = trTextpdf[46];
    strData[5] = trTextpdf[47];
    strData[6] = trTextpdf[48];
    strData[7] = trTextpdf[49];

    tableInfo = commonPdf->PrepareTable(
                QPoint(Pos_x , Pos_y), Rows+1 , TEST_EVENT_COL , trTextpdf[41], strData);
    for(i = 0 ; i < TEST_EVENT_COL ; i++)
    {
        commonPdf->SetColWidth(tableInfo , i , ColWidth[i]);
    }

    commonPdf->CreateTable(tableInfo);

    int typeTemp = 0;
    //Title 不进入计算
    for(i = 1 ; i < Rows+1 ; i++)
    {
        for(j = 0 ; j < TEST_EVENT_COL ; j++)
        {
            strData[j] = TestEventsData[i-1][j];
            if(j == 1)
            {
                typeTemp = TestEventsData[i-1][j].toInt();
                switch(typeTemp)
                {
                case EVENT_SPLICE:
                case EVENT_SPLICE_ANI:
                    strData[j] = trTextpdf[61];
                    break;
                case EVENT_REFLECTION:
                    strData[j] = trTextpdf[62];
                    break;
                case EVENT_END:
                    strData[j] = trTextpdf[63];
                    break;
                default:
                    strData[j] = trTextpdf[61];
                    break;

                }
            }

        }

        commonPdf->InsertRow(tableInfo , strData);
    }

    //PainterPage();
}


