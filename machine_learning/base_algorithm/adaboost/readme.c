#include "mainwindow.h"
#include "ui_mainwindow.h"

#define ACTIVECODE_ERROR "codeErr"
#define MODULES_COUNT 14
QString modulesID[MODULES_COUNT] = {
            "VFL",          //0
            "OPM",          //1
            "Laser",        //2
            "FiberSpot",
            "FiberPath",
            "FiberTune",
            "QAM",
            "TDR",
            "Remote",
            "Worker",
            "Seeker",
            "WiFi",
            "GPS",
            "Bluetooth"
};

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    TabIndex(0)
{
    ui->setupUi(this);

    pSqlActiveDB = new QSqlActiveDataBasse(this);
    pSqlActiveDB->InitializeDB();

    timeActiveMode = 0;
    ui->checkBox_Old->setChecked(true);

    ui->codeEdit->setFont(QFont("Arial",10,QFont::Normal));
    ui->snEdit->setFont(QFont("Arial",10,QFont::Normal));
    ui->verLabel->setText("V1.00.06");

    m_shortCode = true;
    ui->shortCode->setChecked(true);

    name_Cali = QString("Cali");

    CheckBoxAndValue *tempCheck = NULL;

    //for Tab 1 Models ===========================================================
    listTabModel = new QList<CheckBoxAndValue*>;

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_vfl;
    tempCheck->string = QString("VFL");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_opm;
    tempCheck->string = QString("OPM");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_FiberSpot;
    tempCheck->string = QString("Spot");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_FiberPass;
    tempCheck->string = QString("Pass");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_Laser;
    tempCheck->string = QString("Laser");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_FiberPath;
    tempCheck->string = QString("Path");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_Remote;
    tempCheck->string = QString("Remote");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_TDR;
    tempCheck->string = QString("TDR");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_QAM;
    tempCheck->string = QString("QAM");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_USBWIFI;
    tempCheck->string = QString("USBWiFi");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_Worker;
    tempCheck->string = QString("Worker");
    listTabModel->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->model_Seeker;
    tempCheck->string = QString("Seeker");
    listTabModel->append(tempCheck);

    //for tab 2 Langu ===========================================================
    listTabLangu = new QList<CheckBoxAndValue*>;

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->zhongwen;
    tempCheck->string = QString("CN");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->yingwen;
    tempCheck->string = QString("EN");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->ewen;
    tempCheck->string = QString("RUSS");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->yidaliwen;
    tempCheck->string = QString("ITA");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->xibanyawen;
    tempCheck->string = QString("SPAN");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->putaoyawen;
    tempCheck->string = QString("POR");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->fawen;
    tempCheck->string = QString("FR");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->dewen;
    tempCheck->string = QString("GER");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->jiantizhongwen;
    tempCheck->string = QString("CHT");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->riwen;
    tempCheck->string = QString("JAP");
    listTabLangu->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->hanwen;
    tempCheck->string = QString("KOR");
    listTabLangu->append(tempCheck);



    // for Tab 3 Cali ===========================================================
    listTabCali = new QList<CheckBoxAndValue*>;

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->cb_Dist;
    tempCheck->string = QString("Dist");
    listTabCali->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->cb_Dynamic;
    tempCheck->string = QString("Dyna");
    listTabCali->append(tempCheck);

    tempCheck = new CheckBoxAndValue;
    tempCheck->checkBox = ui->cb_Gain;
    tempCheck->string = QString("Gain");
    listTabCali->append(tempCheck);
}

MainWindow::~MainWindow()
{
    pSqlActiveDB->closeDataBase();

    delete ui;
    CheckBoxAndValue *tempCheck = NULL;
    while(listTabCali->count() != 0){
        tempCheck = listTabCali->at(0);
        listTabCali->removeFirst();
        delete tempCheck;
        tempCheck = NULL;
    }
}

QString MainWindow::getCurDesktop()
{
    const int MAX_BUFFER_LEN = 500;
    char  szBuffer[MAX_BUFFER_LEN];
    DWORD dwNameLen;

    dwNameLen = MAX_BUFFER_LEN;
    if (!GetComputerNameA(szBuffer, &dwNameLen))
        qDebug("Error  %d\n", GetLastError());
    else
        qDebug("计算机名为: %s\n", szBuffer);

    dwNameLen = MAX_BUFFER_LEN;
    if (!GetUserNameA(szBuffer, &dwNameLen))
        qDebug("Error  %d\n", GetLastError());
    else
        qDebug("当前用户名为：%s\n", szBuffer);

    ;



    QString strDesktop = QString("C:/Users/");
    strDesktop += QString(szBuffer);
    strDesktop += QString("/Desktop/ActiveCode/");

    QDir dirCur;
    strDesktop=dirCur.currentPath();

    strDesktop += QString("/ActiveCode/");

    QDir dir(strDesktop);
    if(!dir.exists())
    {
        dir.mkdir(strDesktop);//只创建一级子目录，即必须保证上级目录存在
    }
    return strDesktop;

}

void MainWindow::saveActiveCode(QString filePath, QStringList codeList)
{
    char tempBuffer[100] = {'\0'};
    QFile file(filePath);
    if(file.exists()){
        qDebug("remove has Actived same sn file!");
        file.remove();
    }
    if(!file.open(QIODevice::ReadWrite)) // |QIODevice::Append))
        printf("Error open filePath \n");

    for(int i = 0; i < codeList.count(); i++){
        QString tempStr = codeList.at(i);
        sprintf(tempBuffer, "%s \r\n", tempStr.toStdString().c_str());
        file.write(tempBuffer);
    }

    file.close();
}

QString MainWindow::readModuleActiveTimesDB(int snID, int moduleID)
{
    QString times;
    DataBStruct_Active selInfo;
    pSqlActiveDB->cleanDBStruct(&selInfo);
    selInfo.sn = snID;
    selInfo.moduleID = moduleID;

    QList<QStringList> strVecList;
    strVecList.clear();

    strVecList = pSqlActiveDB->selectLines(&selInfo);

    if(strVecList.count() > 0){
        QStringList strlist = strVecList.at(0);
        times = strlist.at(2);
    }else{
        times = "0";
        qDebug(" >>>>>>> should not get here !!");
    }

    return times;
}
/*
Adaboost是一种迭代算法，其核心思想是针对同一个训练集训练不同的分类器(弱分类器)，然后把这些弱分类器集合起来，构成一个更强的最终分类器 (强分类器)。其算法本身是通过改变数据分布来实现的，它根据每次训练集之中每个样本的分类是否正确，以及上次的总体分类的准确率，来确定每个样本的权 值。将修改过权值的新数据集送给下层分类器进行训练，最后将每次训练得到的分类器最后融合起来，作为最后的决策分类器。使用adaboost分类器可以排 除一些不必要的训练数据特徵，并将关键放在关键的训练数据上面。
　　目前，对adaboost算法的研究以及应用大多集中于分类问题，同时近年也出 现了一些在回归问题上的应用。就其应用adaboost系列主要解决了: 两类问题、 多类单标签问题、多类多标签问题、大类单标签问题，回归问题。它用全部的训练样本进行学习。
　　该算法其实是一个简单的弱分类算法提升过程，这个过程通过不断的训练，可以提高对数据的分类能力。整个过程如下所示：
　　1. 先通过对N个训练样本的学习得到第一个弱分类器 ；
　　2. 将 分错的样本和其他的新数据一起构成一个新的N个的训练样本，通过对这个样本的学习得到第二个弱分类器 ；
　　3. 将 和 都分错了的样本加上其他的新样本构成另一个新的N个的训练样本，通过对这个样本的学习得到第三个弱分类器 ；
　　4. 最终经过提升的强分类器 。即某个数据被分为哪一类要通过 ， ……的多数表决。
　　2.3 Adaboost(Adaptive Boosting)算法
　　对于boosting算法，存在两个问题：
　　1. 如何调整训练集，使得在训练集上训练的弱分类器得以进行；
　　2. 如何将训练得到的各个弱分类器联合起来形成强分类器。
　　针对以上两个问题，adaboost算法进行了调整：
　　1. 使用加权后选取的训练数据代替随机选取的训练样本，这样将训练的焦点集中在比较难分的训练数据样本上；
　　2. 将弱分类器联合起来，使用加权的投票机制代替平均投票机制。让分类效果好的弱分类器具有较大的权重，而分类效果差的分类器具有较小的权重。
　　Adaboost算法是Freund和Schapire根据在线分配算法提出的，他们详细分析 了Adaboost算法错误率 的上界，以及为了使强分类器 达到错误率 ，算法所需要的最多迭代次数等相关问题。与Boosting算法不同的是，adaboost算法不需要预先知道弱学习算法学习正确率的下限即弱分类器的误 差，并且最后得到的强分类器的分类精度依赖于所有弱分类器的分类精度， 这样可以深入挖掘弱分类器算法的能力。
　　Adaboost算法中不同的训练集是通过调整每个样本对应的权重来实现的。
开始时，每个样本 对应的权重是相同的，即 其中 n 为样本个数，在此样本分布下训练出一弱分类器 。
对于 分类错误的样本，加大其对应的权重；而对于分类正确的样本，降低其权重，这样分错的样本就被突出出来，从而得到一个新的样本分布 。
在新的样本分布下，再次对弱分类器进行训练，得到弱分类器。依次类推，经过 T 次循环，得到 T 个弱分类器，
把这 T 个弱分类器按一定的权重叠加（boost）起来，得到最终想要的强分类器。

　　Adaboost算法的具体步骤如下：
　　1. 给定训练样本集 ，其中 分别对应于正例样本和负例样本； 为训练的最大循环次数；
　　2. 初始化样本权重 ，即为训练样本的初始概率分布；
　　3. 第一次迭代：
　　(1) 训练样本的概率分布 下，训练弱分类器：
　　(2) 计算弱分类器的错误率：
　　(3) 选取 ，使得 最小
　　(4) 更新样本权重：
　　(5) 最终得到的强分类器：
　　Adaboost算法是经过调整的Boosting算法，其能够对弱学习得到的弱分类器的错误 进行适应性调整。上述算法中迭代了 次的主循环，每一次循环根据当前的权重分布 对样本x定一个分布P，然后对这个分布下的样本使用若学习算法得到一个错误率为 的弱分类器 ，对于这个算法定义的弱学习算法，对所有的 ，都有 ，而这个错误率的上限并不需要事先知道，实际上 。每一次迭代，都要对权重进行更新。更新的规则是：减小弱分类器分类效果较好的数据的概率，增大弱分类器分类效果较差的数据的概率。最终的分类器是 个弱分类器的加权平均。

