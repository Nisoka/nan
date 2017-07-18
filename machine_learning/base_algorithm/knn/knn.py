#!/usr/bin/env python
# -*- coding: utf-8 -*-
# @author liu.junnan

from numpy import *
import operator
from os import listdir

'''
    inX : Vector to classify    (1XN N feature count)
    dataSet: size m Count data of known labels Vectors(MxN)
    labels: dataSet label (1XM)

    tile函数
        在看机器学习实战这本书时，遇到numpy.tile(A,B)函数，愣是没看懂怎么回事，装了numpy模块后，实验了几把，原来是这样子：
    重复A，B次，这里的B可以时int类型也可以是远组类型。
    >>> import numpy
    >>> numpy.tile([0,0],5) #在列方向上重复[0,0]5次，默认行1次
    array([0, 0, 0, 0, 0, 0, 0, 0, 0, 0])
    >>> numpy.tile([0,0],(1,1))#在列方向上重复[0,0]1次，行1次
    array([[0, 0]])
    >>> numpy.tile([0,0],(2,1))#在列方向上重复[0,0]1次，行2次
    array([[0, 0],
           [0, 0]])
    >>> numpy.tile([0,0],(3,1))
    array([[0, 0],
           [0, 0],
           [0, 0]])
    >>> numpy.tile([0,0],(1,3))#在列方向上重复[0,0]3次，行1次
    array([[0, 0, 0, 0, 0, 0]])
    >>> numpy.tile([0,0],(2,3))#在列方向上重复[0,0]3次，行2次</span>
    array([[0, 0, 0, 0, 0, 0],
           [0, 0, 0, 0, 0, 0]])
'''
'''
    KNN     K近邻分类算法
    没有训练过程，直接比较与训练数据集中个点的距离。
    取前K个最近的点, 票决出属于哪个类别.
    输入:
        inX     待分类 inX
        dataSet 训练数据集mat
        labels  数据集标签list
        k       使用前k临近点进行票决

    内容:
        1 计算inX 到 dataSet 所有点的欧氏距离 sqDiffMat.sum(axis=1) ** 0.5
        2 for i in range(k): 保存 前K个最近点的label 进行累加计算 得到列表
        3 票决
'''
def classify0(inX, dataSet, labels, k):
    #dataSet.shape == [mxn](m个 n特征样本)
    dataSetSize = dataSet.shape[0]
    #tile 扩展inX [1xn] 为 [mxn] - dataSet 得到 与 dataSet 每个样本各个特征的差值[ [feat1, feat2, feat3] ]
    diffMat = tile(inX, (dataSetSize, 1)) - dataSet
    #(x1-x2)^2  (y1-y2)^2
    sqDiffMat = diffMat**2
    #求解距离, (x1-x2)^2 + (y1-y2)^2 得到距离列向量[mx1]
    sqDistances = sqDiffMat.sum(axis = 1)
    distances = sqDistances**0.5
    # 排序 按从小到大排序
    sortedDistIndicies = distances.argsort()

    # 去前k个 距离最近的样本点,按类型名为字典key 出现次数为value
    classCount = {}
    for i in range(k):
        voteIlabel = labels[sortedDistIndicies[i]]
        classCount[voteIlabel] = classCount.get(voteIlabel, 0) + 1
    #按 value 大小进行排序
    #sorted( 迭代器(list), key=待排序的关键元素因子, reverse=反转与否(默认小->大))
    sortClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
    return sortClassCount[0][0]





class cKnn:
    def __init__(self, data_x, data_y):
        self.data_x = data_x
        self.data_y = data_y

    def train(self):
        return True

    def predict(self, entry_x, k):
        ret = classify0(entry_x, self.data_x, self.data_y, k)
        return ret

    def trainErr():
        print("KNN don't need train and has no train Err!")
        return 0.0







# 这个loadDataSet 实现的不好 别的loadData 通过append 将每行数据添加到列表再通过转换得到mat.
def loadDataSet(filename):
    fr = open(filename)
    numberOfLines = len(fr.readlines())
    returnMat = zeros((numberOfLines, 3))
    classLabelVector = []
    fr = open(filename)
    index = 0
    for line in fr.readlines():
        listFromLine = line.strip().split('\t')
        returnMat[index, :] = listFromLine[0:3]
        classLabelVector.append(int(listFromLine[-1]))
        index += 1
    return returnMat, classLabelVector

'''
    特征归一化
    每个特征因为上下限不一致，如果使用KNN这样的计算方法
    有些小的特征根本就没有用处了，必须归一化才能使用.
'''
def autoNorm(dataSet):
    # 按 axis = 0 方向(最低维度) 获得最小值 每个特征最小值 最大值。
    # [minA, minB, minC.....]
    # [maxA, maxB, maxC.....]
    # [ragA, ragB, ragC.....]
    minVals = dataSet.min(0)
    maxVals = dataSet.max(0)
    ranges = maxVals - minVals

    normDataSet = zeros(shape(dataSet))
    m = dataSet.shape[0]
    # tile 扩展ndArray, axis = 1 按 高维度方向.
    # xget = (X - min)/(max - min)  归一化到 0-1之间
    normDataSet = dataSet - tile(minVals, (m, 1))
    normDataSet = normDataSet/tile(ranges, (m, 1))
    return normDataSet, ranges, minVals


def dataTest():
    hoRatio = 0.5
    dataSetMat, labels = loadDataSet('datingTestSet2.txt')

    normMat, ranges, minVals = autoNorm(dataSetMat)
    m = normMat.shape[0]
    numTestVectors = int(m*hoRatio)
    errorCount = 0
    # print(normMat[numTestVectors:m, :])

    knn = cKnn(normMat[numTestVectors:m, :], labels[numTestVectors:m])
    # knn.train()
    for i in range(numTestVectors):
        classifyResult = knn.predict(normMat[i, :], 7)
#        print("Testing now classifyResult is %d, labels is %d " %(classifyResult, labels[i]))
        if classifyResult != labels[i]:
            errorCount += 1

    print("Test Error Count is %d in %d" % (errorCount, numTestVectors))



###########################
'''
    MINIST handwrite digital regco
'''
###########################
# 01 image 是 32*32大小的图片 模拟像素
# 通过将32*32像素点读取形成一个 (1, 1024) 大小的行向量.
def img2vector(filename):
    returnVect = zeros( (1, 1024) )
    fr = open(filename)
    for i in range(32):
        line = fr.readline()
        for j in range(32):
            returnVect[0, 32*i + j] = int(line[j])

    return returnVect


def handwritingClassTest():
    hwLabels = []

    # trainSet
    trainingFileList = listdir("trainingDigits")
    m = len(trainingFileList)
    dataMat = zeros( (m, 1024) )
    for i in range(m):
        fileNameStr = trainingFileList[i]
        fileStr = fileNameStr.split('.')[0]
        classLabel = int(fileStr.split('_')[0])
        # classLabelsTrain
        hwLabels.append(classLabel)
        # dataMat
        dataMat[i, :] = img2vector('trainingDigits/%s' % fileNameStr)

    # testSet
    testFileList = listdir("testDigits")
    mTest = len(testFileList)
    errorCount = 0

    print("Begin to KNN!")
    knn = cKnn(dataMat, hwLabels)
    # knn.train()

    for i in range(mTest):
        fileNameStr = testFileList[i]
        fileStr = fileNameStr.split('.')[0]
        label = int(fileStr.split('_')[0])
        testVec = img2vector('testDigits/%s' % fileNameStr)
        # classifyResult = classify0(testVec, dataMat, hwLabels, 7)
        classifyResult = knn.predict(testVec, 7)
        if classifyResult != label:
            print("Error >>>>>>>> %s, classify is %d real is %d" % (fileStr, classifyResult, label))
            errorCount += 1

    print("Error Count is %d in %d" %(errorCount, mTest))



def easyToUse():
    dataTest()
    handwritingClassTest()

if __name__ == "__main__":
    easyToUse()




