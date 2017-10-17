#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *
import matplotlib.pyplot as plt


def loadDataSet(fileName):
    dataMat = []
    labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr = line.strip().split('\t')
        # 每条数据 都增加一个 x0 = 1 其对应的 权重作为线性偏置.
        dataMat.append([1.0, float(lineArr[0]), float(lineArr[1])])
        labelMat.append(int(lineArr[2]))
    return dataMat, labelMat

# sigmoid函数将线性分类 非线性化 1/(1+exp(x))
def sigmoid(inX):
    return longfloat(1.0/(1 + exp(-inX)))

'''
    梯度下降算法
    in:
        dataMatIn   训练数据集
            [[], []]
            [] 是 特征向量, 并加上一个偏置位x0=1
        labelMatIn  数据标签
    precess:
        1 初始化weights 都为1 alpha = 0.1
        2 迭代进行梯度下降maxIter
        3 计算yHat = 1/(1+exp(-X*W))
        4 梯度方向更新weidghts wi = wi + alpha*xi*(y-yHat)

'''

# dataMatIn存放的是3个特征，是100*3的矩阵
# classLabels存放的是类别标签，是1*100的行向量
#def gradAscent(dataMatIn,classLabels):
#    # 转换为NumPy矩阵数据类型
#    dataMatrix=mat(dataMatIn)
#    labelMat=mat(classLabels).transpose()
#    m,n=shape(dataMatrix)
#    alpha=0.001 # 向目标移动的步长
#    maxCycles=500 # 迭代次数
#    weights=ones((n,1))
#    for k in range(maxCycles):
#        #　矩阵相乘
#        h=sigmoid(dataMatrix*weights) # 列向量的元素个数等于样本个数
#        error=(labelMat-h)
#        weights=weights+alpha*dataMatrix.transpose()*error
#    # getA() Return self as an ndarray object.
#    return weights.getA()

def gradAscent(dataMatIn, labelMatIn, maxIter = 500):
    dataMat = mat(dataMatIn)
    labelMat = mat(labelMatIn).transpose()
    m, n = shape(dataMat)
    alpha = 0.001
    weights = ones( (n, 1) )
#    print(dataMat)
#    print(weights)

    for iter in range(maxIter):
        h = sigmoid(dataMat*weights)
        error = labelMat - h
        weights = weights + alpha*dataMat.transpose()*error
    return weights


def plotBestFit(dataMat, labelMat, weights):
    #dataMat, labelMat = loadDataSet('testSet.txt')
    dataArr = array(dataMat)
    n = shape(dataArr)[0]
    xcord1 = []
    ycord1 = []
    xcord2 = []
    ycord2 = []
    for i in range(n):
        if int(labelMat[i]) == 1:
            xcord1.append(dataArr[i, 1])
            ycord1.append(dataArr[i, 2])
        else:
            xcord2.append(dataArr[i, 1])
            ycord2.append(dataArr[i, 2])
    fig = plt.figure()
    ax = fig.add_subplot(1, 1, 1)
    ax.scatter(xcord1, ycord1, s = 30, c = 'red', marker = 's')
    ax.scatter(xcord2, ycord2, s = 30, c = 'blue', marker = 's')
    x = arange(-3.0, 3.0, 0.1)
    y = (-float(weights[0]) - float(weights[1])*x)/float(weights[2])
#    print(x, y)
    ax.plot(x, y)
    plt.show()

def classifyVector(inX, weights):
    predict = sigmoid(sum(inX*weights))
    if predict > 0.5:
        return 1.0
    else:
        return 0.0

class cLogisticReg:
    def __init__(self, data_x, data_y, maxIter = 500):
        self.data_x = data_x
        self.data_y = data_y
        self.maxIter = maxIter
        self.m = shape(data_x)[0]
        self.weights = ones((self.m, 1))

    def train(self):
        self.weights = gradAscent(self.data_x, self.data_y, self.maxIter)
        return self.weights

    def predict(self, entry_x):
        ret = classifyVector(entry_x, self.weights)
        return ret

    def trainErr(self):
        errCnt = 0
        for i in range(self.m):
            entry_x = self.data_x[i]
            result = self.predict(entry_x)
            if result != self.data_y[i]:
                print(result, self.data_y[i])
                errCnt += 1
#        print(errCnt, self.m)
        return errCnt/float(self.m)

    def plotTrainResult(self):
        plotBestFit(self.data_x, self.data_y, self.weights)




###############
# Test
###############


def dataSetTest():
    dataMat, labelMat = loadDataSet('testSet.txt')
    Weights = gradAscent(mat(dataMat), mat(labelMat))
    print(Weights)
    errCnt = 0
    for i in range(len(dataMat)):
        result = classifyVector(dataMat[i], Weights)
        if result != labelMat[i]:
            print(result, labelMat[i])
            errCnt += 1
    print(errCnt)
    #plotBestFit(dataMat, labelMat, Weights)

#    logisReg = cLogisticReg(dataMat, labelMat, 700)
#    Weights = logisReg.train()
#    logisReg.trainErr()
    # logisReg.plotTrainResult()







def colicTest():
    frTrain = open('horseColicTraining.txt')
    trainingSet = []
    trainingLabels = []
    for line in frTrain.readlines():
        curLine = line.strip().split('\t')
        lineArr = []
        for i in range(21):
            lineArr.append(float(curLine[i]))
        trainingSet.append(lineArr)
        trainingLabels.append(float(curLine[21]))

    logisReg = cLogisticReg(trainingSet, trainingLabels, 800)
    trainWeights = logisReg.train()
    logisReg.trainErr()
    #trainWeights = gradAscent(mat(trainingSet), mat(trainingLabels))

    frTest = open('horseColicTest.txt')
    errorCount = 0
    numTestVec = 0
    for line in frTest.readlines():
        numTestVec += 1
        curLine = line.strip().split('\t')
        lineArr = []
        for i in range(21):
            lineArr.append(float(curLine[i]))
        classifyResult = logisReg.predict(array(lineArr))
        if classifyResult != int(curLine[21]):
            print("Error >>>>>>>>>>>>> %d line" % numTestVec)
            errorCount += 1
    print("Error All the Error Cnt is %d in %d" % (errorCount, numTestVec))

    return errorCount




def easyToUse():
    dataSetTest()
#    colicTest()

if __name__ == "__main__":
    easyToUse()
