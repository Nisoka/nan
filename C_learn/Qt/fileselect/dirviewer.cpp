#include "dirviewer.h"
#include "ui_dirviewer.h"

DirViewer::DirViewer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DirViewer)
{
    ui->setupUi(this);

    m_loop = NULL; //new line
    //
    setGeometry(0, 0, 800, 440);

    //
    model = new QFileSystemModel;
    model->setFilter(QDir::Files);
    model->setReadOnly(true);
    model->setRootPath("/opt/user/document/");


    treeView = new QTreeView(this);
    treeView->setModel(model);

    treeView->setSortingEnabled(true);
    treeView->setWindowFlags(Qt::FramelessWindowHint);
    treeView->setGeometry(0, 40, 700, 440);
    treeView->header()->setStretchLastSection(true);
    treeView->header()->setSortIndicator(0, Qt::AscendingOrder);
    treeView->header()->setSortIndicatorShown(true);
    treeView->header()->setClickable(true);

    treeView->setColumnWidth(0, 350);
    treeView->setColumnWidth(1, 70);
    treeView->setColumnWidth(2, 70);
    treeView->setColumnWidth(3, 190);
    treeView->setItemDelegate(new MyDelegate);


    QModelIndex index = model->setRootPath("/opt/user/document");
    treeView->setRootIndex(index);
    treeView->hide();


    //
    OpenButtonP = new PushBtn(this);
    OpenButtonP->setGeometry(705, 0, 90, 50);
    OpenButtonP->UpText=trText[221];
    OpenButtonP->DownText=trText[221];
    OpenButtonP->UpMap=MAX711_PATH+Interface_Type_PATH+MaintainWinStartBtnUpMap;
    OpenButtonP->DownMap=MAX711_PATH+Interface_Type_PATH+MaintainWinStartBtnDownMap;
    OpenButtonP->Check_State = TRUE;
    OpenButtonP->show();

//    SearchButtonP = new PushBtn(this);
//    SearchButtonP->setGeometry(705, 0+(50+10), 90, 50);
//    SearchButtonP->UpText=trText[245];
//    SearchButtonP->DownText=trText[245];
//    SearchButtonP->UpMap=MAX711_PATH+Interface_Type_PATH+MaintainWinStartBtnUpMap;
//    SearchButtonP->DownMap=MAX711_PATH+Interface_Type_PATH+MaintainWinStartBtnDownMap;
//    SearchButtonP->Check_State = TRUE;
//    SearchButtonP->hide();

    ExitButtonP = new PushBtn(this);
    ExitButtonP->setGeometry(705, 0+(50+10)*6+30, 90, 50);
    ExitButtonP->UpText=trText[213];
    ExitButtonP->DownText=trText[213];
    ExitButtonP->UpMap=MAX711_PATH+Interface_Type_PATH+MaintainWinStartBtnUpMap;
    ExitButtonP->DownMap=MAX711_PATH+Interface_Type_PATH+MaintainWinStartBtnDownMap;
    ExitButtonP->Check_State = TRUE;
    ExitButtonP->show();

    connect(OpenButtonP, SIGNAL(MYmousePress()), this->parentWidget(), SLOT(SetBackFromWinID()));
    connect(OpenButtonP, SIGNAL(MYmousePress()), this, SLOT(DealOpenFile()));
    connect(ExitButtonP, SIGNAL(MYmousePress()), this, SIGNAL(msgExit()));
    connect(ExitButtonP, SIGNAL(MYmousePress()), treeView, SLOT(hide()));
    connect(ExitButtonP, SIGNAL(MYmousePress()), this, SLOT(on_Exit()));
    //connect(treeView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(DealFileDoubleClicked(QModelIndex)));    //获取文件名时有bug
}

DirViewer::~DirViewer()
{
    delete ui;
}

void DirViewer::DealOpenFile()
{
    QModelIndex selIndex = treeView->currentIndex();
    if(!selIndex.isValid())
        return ;

    selIndex = selIndex.sibling(selIndex.row(), 0);
    QString selFileName = treeView->model()->itemData(selIndex).values()[0].toString();
    qDebug() << "selFileName = " << selFileName;

    m_result = Accepted;
    treeView->hide();
    this->hide();
    m_retPath = dirPath + "/" + selFileName;
    emit msgOpenFile(m_retPath);
}

void DirViewer::DealFileDoubleClicked(QModelIndex selIndex)
{
    if(!selIndex.isValid()){
        return;
    }

    QString selFileName = treeView->model()->itemData(selIndex).values()[0].toString();

    qDebug() << "selFileName = " << selFileName;

    m_result = Accepted;
    treeView->hide();
    this->hide();
    m_retPath = dirPath + "/" + selFileName;
    emit msgOpenFile(dirPath + "/" + selFileName);

}

void DirViewer::hide()
{
    exec_exit();
    QWidget::hide();
}

void DirViewer::on_Exit()
{
    m_result = Rejected;
    hide();
}

void DirViewer::SetDirPath(QString path)
{
    qDebug() << "SetDirPath: " << path;
    dirPath = path;
    QModelIndex index = model->setRootPath(dirPath);
    treeView->setRootIndex(index);
    treeView->show();
}

void DirViewer::open()
{
    setWindowModality(Qt::WindowModal);
    show();
}

int DirViewer::exec()
{
    setAttribute(Qt::WA_ShowModal, true);
    show();

    QEventLoop loop;
    m_loop = &loop; //new line
    loop.exec();
    m_loop = NULL; //new line
    return m_result;
}

void DirViewer::exec_exit()
{
    if(m_loop != NULL)
        m_loop->exit();
}
