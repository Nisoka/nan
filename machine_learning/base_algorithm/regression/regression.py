#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *
import matplotlib.pyplot as plt

def loadDataSet(fileName):      #general function to parse tab -delimited floats
    numFeat = len(open(fileName).readline().split('\t')) - 1 #get number of fields 
    dataMat = [] 
    labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr =[]
        curLine = line.strip().split('\t')
        for i in range(numFeat):
            lineArr.append(float(curLine[i]))
        dataMat.append(lineArr)
        labelMat.append(float(curLine[-1]))
    return dataMat,labelMat


# 正规方程法  xTx的逆 直接求解 weights权重参数
def standRegres(xArr, yArr):
    xMat = mat(xArr)
    yMat = mat(yArr).T
    xTx = xMat.T*xMat
    if linalg.det(xTx) == 0.0:
        print("This matrix is singular, cannot do inverse")
        return 
    ws = xTx.I * (xMat.T*yMat)
    return ws



########################
# 局部线性回归
########################

'''
     局部权重线性回归
     有点类似于 KNN 算法 不进行训练学习，直接用数据进行预测.
     in:
         testPoint      待测点
         xArr           训练数据集
         yArr           数据值
    process:
        1 初始化权重 weights 为 eye(m) 单位矩阵.
        2 根据testPoint 距离 训练数据集中各点距离 diffMat
            计算各点对testPoint预测的权重影响，距离越近(或者说点越相似) 权重越大影响越大
        3 更新训练数据集 的xTx = xMat.T*xMat == > xTx = xMat.T*(weights*xMat)
        4 得到对于 testPoint点的个性化 ws权重线性函数，直接预测值即可。
'''
def lwlr(testPoint, xArr, yArr, k=1.0):
    xMat = mat(xArr)
    yMat = mat(yArr).T
    m = shape(xMat)[0]
    weights = mat(eye(m))
    
    for j in range(m):
        diffMat = testPoint - xMat[j, :]
        weights[j, j] = exp(diffMat*diffMat.T/(-2.0*k**2))
    xTx = xMat.T * (weights * xMat)

    if linalg.det(xTx) == 0.0:
        print("This matrix is singular, cannot do inverse")
        return 
    ws = xTx.I *(xMat.T * (weights * yMat))
    return testPoint*ws

def lwlrTest(testArr, xArr, yArr, k=1.0):
    m = shape(testArr)[0]
    yHat = zeros(m)
    for i in range(m):
        yHat[i] = lwlr(testArr[i], xArr, yArr, k)
    return yHat

#def lwlrTestPlot(xArr, yArr, k=1.0):
#    yHat = zeros(shape(yArr))
#    xCopy = mat(xArr)
#    xCopy.sort(0)
#    for i in range(shape(xArr)[0]):
#        yHat[i] = lwlr(xCopy[i], xArr, yArr, k)
#    return yHat, xCopy


def lwlrTestPlot():
    xArr, yArr = loadDataSet('ex0.txt')
    yHat = lwlrTest(xArr, xArr, yArr, 0.01)
    xMat = mat(xArr)
    srtInd = xMat[:, 1].argsort(0)
    xSort = xMat[srtInd][:, 0, :]
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.plot(xSort[:, 1], yHat[srtInd])
    ax.scatter(xMat[:, 1].flatten().A[0], mat(yArr).T.flatten().A[0], s=2, c='red')
    plt.show()




########################
# 岭回归
########################
#(yhat - y)^2 最小二乘误差
def rssError(yArr,yHatArr): #yArr and yHatArr both need to be arrays
    return ((yArr-yHatArr)**2).sum()

'''
 岭回归 普通最小二乘回归 + 惩罚系数(岭系数)
    xMat加上惩罚系数, 会使得 xMat.T*xMat 总可求逆
    齐次线性方程组有关系
 进而可以通过 xTx 的逆 求得 回归系数
'''

def ridgeRegres(xMat,yMat,lam=0.2):
    xTx = xMat.T*xMat
    denom = xTx + eye(shape(xMat)[1])*lam
    if linalg.det(denom) == 0.0:
        print("This matrix is singular, cannot do inverse")
        return

#    print(denom.shape)
#    print(xMat.shape)
#    print(yMat.shape)
    ws = denom.I * (xMat.T*yMat)
    return ws
    
def ridgeTest(xArr,yArr):
    #特征收缩, 得到以0为均值 1 为方差的特征矩阵？？
    xMat = mat(xArr)
    xMeans = mean(xMat,0)
    xVar = var(xMat,0)
    xMat = (xMat - xMeans)/xVar

    yMat = mat(yArr).T
    yMean = mean(yMat,0)
    yMat = yMat - yMean

    numTestPts = 30
    wMat = zeros((numTestPts,shape(xMat)[1]))
    for i in range(numTestPts):
        ws = ridgeRegres(xMat,yMat,exp(i-10))
        wMat[i,:]=ws.T
    return wMat





############################
# 前向步进线性回归
############################

# 特征收缩, 与归一化不同 但是作用是什么？
# mean - 求解均值    u
# var  - 求解标准差  s
# xNew = (x - u)/s
def regularize(xMat):#regularize by columns
    inMat = xMat.copy()
    inMeans = mean(inMat,0)   #calc mean then subtract it off
    inVar = var(inMat,0)      #calc variance of Xi then divide by it
    inMat = (inMat - inMeans)/inVar
    return inMat

# 前向步进线性回归(不使用梯度下降、不使用正规方程组、直接进行Loss减少)
def stageWise(xArr, yArr, eps=0.01, numIt=100):
    xMat = mat(xArr)
    xMat = regularize(xMat)

    yMat = mat(yArr).T
    yMean = mean(yMat, 0)
    yMat = yMat - yMean

    m, n = shape(xMat)

    returnMat = zeros((numIt,n))
    ws = zeros((n, 1))
    wsTest = ws.copy()
    wsMax = ws.copy()
    # 进行numIt次 更新ws向量
    for i in range(numIt):
        #print(ws.T)
        lowestErr = inf;
        # 逐个特征 进行更新ws_n
        for j in range(n):
            # 两个方向,逐个方向测试那个能够减少误差
            for sign in [-1,1]:
                wsTest = ws.copy()
                wsTest[j] += eps*sign
                yTest = xMat*wsTest
                rssE = rssError(yMat.A, yTest.A)
                if rssE < lowestErr:
                    lowestErr = rssE
                    wsMax = wsTest
        ws = wsMax.copy()
        returnMat[i, :] = ws.T
    return wsMax

####################
'''
# easyToUse
    前向步进线性回归
    岭回归
    标准线性回归
    得到的回归系数打印出来比较

'''
####################
def easyToUse():
    xArr, yArr = loadDataSet('abalone.txt')
    wsStage = stageWise(xArr, yArr, 0.001, 5000)
    print(wsStage)

    xMat = mat(xArr)
    xMat = regularize(xMat)
    yMat = mat(yArr).T
    yMean = mean(yMat, 0)
    yMat = yMat - yMean

    wsRidge = ridgeRegres(xMat, yMat)
    print(wsRidge)

    wsStand = standRegres(xMat, yMat.T)
    print(wsStand)

if __name__ == "__main__":
    easyToUse()
    
