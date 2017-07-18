#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *
import matplotlib.pyplot as plt

def loadDataSet(fileName, delim = '\t'):
    fr = open(fileName)
    dataSet = []
    stringArr = [line.strip().split(delim) for line in fr.readlines()]
    for lineArr in stringArr:
        curLine = []
        for nField in lineArr:
            curLine.append(float(nField))
        dataSet.append(curLine)
    return mat(dataSet)

def pca(dataMat, topNfeat = 9999):
    meanVals = mean(dataMat, axis = 0)
    meanRemoved = dataMat - meanVals
    # 计算协方差矩阵covMat
    covMat = cov(meanRemoved, rowvar=0)
    # 计算covMat的特征值 以及 特征向量 列表
    eigVals, eigVects = linalg.eig(mat(covMat))
    eigValInd = argsort(eigVals)
    # 选取前 topN 的特征值
    eigValInd = eigValInd[:(-topNfeat+1):-1]
    # 获得 topN 特征向量 构建成 转换基(转换基的数目就是映射后的特征维度)
    redEigVects = eigVects[:, eigValInd]
    lowDDataMat = meanRemoved * redEigVects
    # 映射到 原本基 中，不过就是重新映射了值 发现在转换基方向上的映射量不变，但是原本基中的映射量变小，这就是 特征向量映射、降维 不会减少数据信息的原因，在降维过程中信息量并没有实际变小，不过是在原本基上进行了变换
    reconMat = (lowDDataMat * redEigVects.T) + meanVals
    return lowDDataMat, reconMat
    
def replaceNanWithMean(): 
    datMat = loadDataSet('secom.data', ' ')
    numFeat = shape(datMat)[1]
    for i in range(numFeat):
        #values that are not NaN (a number)
        meanVal = mean(datMat[nonzero(~isnan(datMat[:,i].A))[0],i])
        #set NaN values to mean
        datMat[nonzero(isnan(datMat[:,i].A))[0],i] = meanVal 
    return datMat
        

def easyToUse():
    dataMat = loadDataSet('testSet.txt')
    lowDataMat, reconMat = pca(dataMat, 1)

    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.scatter(dataMat[:,0].A, dataMat[:,1].A, marker='^', s = 90)
    ax.scatter(reconMat[:,0].A, reconMat[:,1].A, marker='^', s = 90)
    plt.show()
    print(lowDataMat)
    print(reconMat)

def semiTest():
    dataMat = replaceNanWithMean()
    meanVals = mean(dataMat, axis=0)
    meanRemoved = dataMat - meanVals
    covMat = cov(meanRemoved, rowvar=0)
    eigVals, eigVects = linalg.eig(mat(covMat))
    arr = linspace(5, 80, 16)
    covSum = sum(eigVals)
    print(covSum)
    sumList = []
    for i in arr:
        print(i)
        tempSum = sum(eigVals[0:int(i)])
        sumList.append(tempSum/covSum)

    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.scatter(arr, sumList, marker='^', s = 90)
    plt.plot(linspace(0, 99, 100), ones((100)), c = 'red')
    plt.show()
    
    
        

if __name__ == "__main__":
    semiTest()

