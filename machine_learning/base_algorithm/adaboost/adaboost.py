#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *
import matplotlib.pyplot as plt

def loadSimpData():
    dataMat = matrix([[1., 2.1],
        [2., 1.1],
        [1.3, 1.],
        [1., 1.],
        [2., 1.]])
    classLabels = [1.0, 1.0, -1.0, -1.0, 1.0]
    return dataMat, classLabels

def loadDataSet(fileName):
    numFeat = len(open(fileName).readline().split('\t'))
    dataMat = []
    labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr = []
        curLine = line.strip().split('\t')
        for i in range(numFeat-1):
            lineArr.append(float(curLine[i]))
        dataMat.append(lineArr)
        labelMat.append(float(curLine[-1]))
    return dataMat, labelMat


'''
 stump 树桩分类算法 -- 基本决策法
 输入:
         dataMatrix     数据集
            Mat
         dimen          特征维度
            int
         threshVal      门限值
            int
         threshIneq > < 决定正负方向
            string
'''
def stumpClassify(dataMatrix, dimen, threshVal, threshIneq):
    retArray = ones( (shape(dataMatrix)[0], 1) )
    if threshIneq == 'lt':
        retArray[dataMatrix[:, dimen] <= threshVal] = -1.0
    else:
        retArray[dataMatrix[:, dimen] > threshVal] = -1.0

    return retArray

'''
    Stump 最佳决策树桩分类算法
    输入:
        dataArr     训练数据集
            list[list, list]
        classLabels 数据集标签
            list_vec 行向量
        D           数据集中权重
            list_vec 列向量
    输出:
        决策树桩stump  结构体 实际上就是 （特征维度 + 决策值 + 决策方向）
'''
def buildStump(dataArr, classLabels, D):
    dataMatrix = mat(dataArr)
    labelMat = mat(classLabels).T
    m, n = shape(dataMatrix)
    numSteps = 10.0
    bestStump = {}
    bestClasEst = mat(zeros( (m, 1) ))
    minErr = inf
    # 逐个特征维度 dimen
    for i in range(n):
        rangeMin = dataMatrix[:, i].min()
        rangeMax = dataMatrix[:, i].max()
        stepSize = (rangeMax-rangeMin)/numSteps
        #逐个 threshVal
        # ?? 为什么是-1 作为开始位置呢？
        for j in range(-1, int(numSteps)+1):
            # 逐个 inequal left right
            for inequal in ['lt', 'gt']:
                threshVal = (rangeMin + float(j)*stepSize)
                # 预测结果向量.
                predictVals = stumpClassify(dataMatrix, i, threshVal, inequal)
                # errArr 分类误差向量 - 有错误=1
                errArr = mat(ones((m, 1)))
                errArr[predictVals == labelMat] = 0

                # 安权重得到 -- 权重错误！
                # weightedErr, 权重误差值
                weightedErr = D.T*errArr
                # 遍历找到 weightedErr 权重决策误差值最小的 决策树桩。
                if weightedErr < minErr:
                    minErr = weightedErr
                    bestClasEst = predictVals.copy()
                    bestStump['dim'] = i
                    bestStump['thresh'] = threshVal
                    bestStump['ineq'] = inequal
    # 返回 决策树桩, 权重决策误差值, 分类结果
    return bestStump, minErr, bestClasEst

'''
    构建 adaBoost 算法   DS 代表单层决策树
    以树桩决策为基本分类器
    输入:
        dataArr     数据集
            mat
        classLabels 标签
            行向量
        numIt       最大弱分类器数量（防止过拟合）
    内容:
        1 weakClassArr  多个弱分类器
        2 D             当前弱分类器对应 数据权重分布
        3 buildStump    构建基本弱分类器
        4 alpha bestStump['alpha']  基本弱分类器权重
        5 D_new         更新数据集权重分布
        6 aggClassEst aggErrors     前向加法计算当前已获得的分类器 分类错误、分类误差。
        7 判断弱分类器已经足够。
'''
def adaBoostTrainDS(dataArr, classLabels, numIt = 40):
    # 弱分类器列表
    weakClassArr = []
    m = shape(dataArr)[0]
    # 初始化基础权重D
    D = mat(ones((m, 1))/m)
    aggClassEst = mat(zeros( (m, 1) ))
    for i in range(numIt):
        # step 1 构建弱分类器(data, labels, D权重)
        #     bestStump 本次弱分类器
        #     error     分类错误率
        #     classEst  分类结果[正确为0,错误为1]
        bestStump, error, classEst = buildStump(dataArr, classLabels, D)

        # step 2 计算分类器在分类器链 中的权重alpha.
        # 分类器权重alpha 是在后来分类器未构建之前得到的，所以只根错误率有关
        alpha = float(0.5*log( (1.0 - error)/max(error, 1e-16)))
        bestStump['alpha'] = alpha
        # 记录弱分类器
        weakClassArr.append(bestStump)

        # step 3 更新数据集权重D
        expon = multiply(-1*alpha*mat(classLabels).T, classEst)
        D = multiply(D, exp(expon))
        D = D/D.sum()

        # step 4 前向加法计算 boost 累积权重分类错误结果
        aggClassEst += alpha*classEst
        aggErrors = multiply(sign(aggClassEst) != mat(classLabels).T, ones((m, 1)))

        # 判断结束
        errorRate = aggErrors.sum()/m
        print("Error rate is %d" % errorRate)
        if errorRate == 0:
            break
    return weakClassArr, aggClassEst

'''
    adaClassify
        利用  adaBoost算法训练出来分类器 进行分类。
    输入:
        dataToClass     待分类数据集
            mat
        classifierArr   stump boost分类器
'''
def adaClassify(dataToClass, classifierArr):
    dataMatrix = mat(dataToClass)
    m = shape(dataMatrix)[0]
    aggClassEst = mat(zeros((m, 1)))

    # 前向分布加法计算 每个基本分类器分类结果 得到整体分类结果
    for i in range(len(classifierArr)):
        classEst = stumpClassify(dataMatrix, classifierArr[i]['dim'], \
                                 classifierArr[i]['thresh'],\
                                 classifierArr[i]['ineq'])
        aggClassEst += classifierArr[i]['alpha'] * classEst
        # print(aggClassEst)
    return sign(aggClassEst)



'''
    类  stump AdaBoost
    __init__
        data_x  data_y  maxIter
        m           数据行数
        maxIter     这里表示 最大分类器数目，太多可能过拟合
        weakClassifiers 分类器集合
            list[dict_Class, list_Class]
            dict_Class['alpha': alpha, 'dim': dimen, 'thresh':thresholdVal, 'ineq': ]
        trainEst    训练预测结果向量 - 并不是错误向量

    train()
        训练函数
            主要调用 adaBoostTrainDS(data_x, data_y, maxIter)
            训练得到 分类器列表  以及结果预测结果

    predict(entry_x)
        预测函数
            对entry_x 调用adaBoost训练好的分类器列表进行 分类预测
        entry_x     与 data_x 特征相同的 实例矩阵 - 包含多个记录
'''
class cStumpAdaBoost:
    def __init__(self, data_x, data_y, maxIter):
        self.data_x = data_x
        self.data_y = data_y
        self.m      = shape(data_x)[0]
        self.maxIter = maxIter
        self.weakClassifiers = []
        self.trainEst = []

    def train(self):
        self.weakClassifiers, self.trainEst = adaBoostTrainDS(self.data_x, self.data_y, self.maxIter)
        return self.weakClassifiers

    def predict(self, entry_x):
        ret = adaClassify(entry_x, self.weakClassifiers)
        return ret

    def trainError(self):
        # print(sign(self.trainEst) != mat(self.data_y).T) 返回的确实是 True False 矩阵
        trainError = multiply(sign(self.trainEst) != mat(self.data_y).T, ones((self.m, 1)))
        # print(trainError) 结果是 0 1 矩阵.
        # 说明 True Flash 矩阵 与 0 1 进行直乘 结果是 0 1矩阵
        # 说明 True False 在python 中 应该是直接用 1, 0 代替的。
        errorRate = trainError.sum()/float(self.m)
        return float(errorRate)



def easyToUse_old():
    dataArr, labelArr = loadSimpData()
    classifierArr, aggClassEst = adaBoostTrainDS(dataArr, labelArr, 30)
    adaClassify([0, 0], classifierArr)
    adaClassify([[5, 5], [0, 0]], classifierArr)


def horsePredict():
    print("This is HorseColicTraining dataSet for test adaBoost base on stump !")
    dataArr, labelArr = loadDataSet('horseColicTraining2.txt')
    testArr, testLabels = loadDataSet('horseColicTest2.txt')

    stumpAdaBoost = cStumpAdaBoost(dataArr, labelArr, 5)
    classifierArr = stumpAdaBoost.train()
    trainError = stumpAdaBoost.trainError()
    prediction = stumpAdaBoost.predict(testArr)

    error = mat(ones((67, 1)))
    errCnt = error[prediction != mat(testLabels).T].sum() 
    print("Err Cnt is %d, Count is %d" % (errCnt, 67))
    

def easyToUse():
    horsePredict()

'''
    what is ROC????
    非均衡分类,
        有些情况如果判断错误 会有较大影响时候。
    使用普通的错分样本比例, 这样的度量错误掩盖了样例如何被分错的事实
    混淆矩阵 --
            猫   狗   鼠
      猫    24   2    5
      狗    2    27   0
      鼠    4    2    30

        利用混淆矩阵就可以更好地理解分类中的错误了
        以及分类错误情况(将猫 分类为狗, 或者将狗分类为猫)。
        如果矩阵中的非对角元素均为0，就会得到一个完美的分类器。

    在这个二类问题中，
            +1      -1
       +1   TP      FN
       -1   FP      TN

    真正例（TmePositive, TP ,也称真阳）；
        如果将一个正例判为正例，那么就可以认为产生了一个

    真反例(TrueNegative, TN ,也称真阴）
        如果对一个反例正确地判为反例，则认为产生了一个

    伪反例（FalseNegative,FN ,也称假阴）和伪正例（FalsePositive, FP ,也称假阳）


    正确率 召回率
        在分类中，当某个类 重要性高于其他类别时，我们就可以利用上述定义来定义出多个比错
    误率更好的新指标。
    正确率(Precision) 它等于TP/TP+FP)，
        给出的是预测为正例的样本中的 真正正例的比例。
    召回率(ReCall)    它等于TP/(TP+FN)
        给出的是预测为正例的真实正例占所有 预测正确实例的比例。
        在召回率很大的分类器中，真正判错的正例的数目并不多。

        我们可以很容易构造一个高正确率或高召回率的分类器，但是很难同时保证两者成立。
    如果将任何样本都判为正例，那么召回率达到百分之百而此时正确率很低。
    构建一个同时使正确率和召回率最大的分类器是具有挑战性的。

        另一个用于度量分类中的非均衡性的工具是ROC曲线（R O C curve )，ROC代表接收者操作特
    征（receiver operating characteristic) ,它最早在二战期间由电气工程师构建雷达系统时使用过。
        在理想的情况下，最佳的分类器应该尽可能地处手左上角，
    这就意味着分类器在假阳率很低的同时获得了很高的真阳率。例如在垃圾邮件的过滤中，这就相当于球滤了所有的垃圾邮件，但
    没有将任何合法邮件误识为垃圾邮件而放入垃圾邮件的文件夹中。
'''
def plotROC(predStrengths, classLabels):
    cur = (1.0,1.0) #cursor
    ySum = 0.0 #variable to calculate AUC
    numPosClas = sum(array(classLabels)==1.0)
    yStep = 1/float(numPosClas); xStep = 1/float(len(classLabels)-numPosClas)
    sortedIndicies = predStrengths.argsort()#get sorted index, it's reverse
    fig = plt.figure()
    fig.clf()
    ax = plt.subplot(111)
    #loop through all the values, drawing a line segment at each point
    for index in sortedIndicies.tolist()[0]:
        if classLabels[index] == 1.0:
            delX = 0; delY = yStep;
        else:
            delX = xStep; delY = 0;
            ySum += cur[1]
        #draw line from cur to (cur[0]-delX,cur[1]-delY)
        ax.plot([cur[0],cur[0]-delX],[cur[1],cur[1]-delY], c='b')
        cur = (cur[0]-delX,cur[1]-delY)
    ax.plot([0,1],[0,1],'b--')
    plt.xlabel('False positive rate'); plt.ylabel('True positive rate')
    plt.title('ROC curve for AdaBoost horse colic detection system')
    ax.axis([0,1,0,1])
    plt.show()
    print("the Area Under the Curve is: ",ySum*xStep)
    


if __name__ == "__main__":
    easyToUse()
        
        




