#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *
import matplotlib.pyplot as plt
from matplotlib.patches import Circle

def loadDataSet(fileName):      #general function to parse tab -delimited floats
    numFeat = len(open(fileName).readline().split('\t'))
    dataMat = []                #assume last column is target value
    fr = open(fileName)
    for line in fr.readlines():
        lineArr = []
        curLine = line.strip().split('\t')
        for i in range(numFeat):
            lineArr.append(float(curLine[i]))
        dataMat.append(lineArr)
    return dataMat

# 欧氏距离(x1a -x1b)**2 + (xa2 - xb2)**2 .. 
def distEclud(vecA, vecB):
    return sqrt(sum(power(vecA - vecB, 2)))

'''
>>> from numpy import *
>>> k = random.rand(5, 1
>>> k
array([[ 0.7078714 ],
       [ 0.731313  ],
       [ 0.59188232],
       [ 0.73207634],
       [ 0.43258146]])

dataMat = mat([[1, 2, 3], [2, 3, 4]])
dataMat.A = array([[1, 2, 3],[2, 3, 4]])

'''
# 生成K个分类中心点
def randCent(dataSet, k):
    n = shape(dataSet)[1]
    centRoids = mat(zeros( (k, n) ))
    for j in range(n):
        minJ = min(dataSet[:, j])
        rangeJ = float(max(dataSet[:, j]) - minJ)
        centRoids[:, j] = mat(minJ + rangeJ*random.rand(k, 1))
    return centRoids

'''
返回
1 centRoids 聚类结果k行[质心位置]
2 clusterAssment  聚类信息 m行[所属, 到质心距离]
'''
def kMeans(dataSet, k, distMeas = distEclud, createCent = randCent):
    m = shape(dataSet)[0]
    clusterAssment = mat(zeros((m, 2)))
    centRoids = createCent(mat(dataSet), k)
    clusterChanged = True

    while clusterChanged:
        clusterChanged = False
        # iterator all the point and assign it to the closest centRoid
        # 更新每个点的群属
        for i in range(m):
            minDist = inf
            minIndex = -1
            for j in range(k):
                distJI = distMeas(centRoids[j, :], dataSet[i, :])
                # 查找最小距离群属
                if distJI < minDist:
                    minDist = distJI
                    minIndex = j

            # I点 的群属 发生改变
            if clusterAssment[i, 0] != minIndex:
                clusterChanged = True
            clusterAssment[i, :] = minIndex, minDist**2
        #print(centRoids)

        # 更新每个族 的位置
        for cent in range(k):
            # 获得该族下每个点
            ptsInClust = dataSet[nonzero(clusterAssment[:, 0].A == cent)[0]]
            # 按最低维求均值(矩阵 只有两个维度 这里表示按列 - 即所有point 的对应特征求均值)
            centRoids[cent, :] = mean(ptsInClust, axis=0)
    return centRoids, clusterAssment


'''
输入:
    dataSet 输入数据集 
        mat类型
    k 待划分类别数
    distMeas 使用计算距离
输出:
    cenList 聚类结果
       mat 类型 k行 [质心特征]
    clusterAssment 聚类结果信息
       mat 类型 m行 [类别ID, 各点距离质心特征距离]
    
'''
'''
    print(mean(dataSet, axis = 0))
    print(centRoid0)
    直接mean 得到的是一个matrix
    [[-0.10361321  0.0543012 ]]
    通过tolist()[0] 得到列表类型
    [-0.10361321250000004, 0.05430119999999998]
'''
'''
bestClusAss[:, 0]              第一列
bestClusAss[:, 0].A            第一列变为Array类型
bestClusAss[:, 0].A == 1       新的Array 做 == 1比较 筛选构成  true false的Array 
nonzero(bestClusAss[:, 0].A == 1)[0]  
    nonzero() 返回生成的trueFalseArray中的True构成的array,内容是 [array, type] 
    所以 nonzero()[0] 得到array. 描述 bestClusAss[:, 0].A == 1条件的array

bestClusAss[array, 0]  ==> 返回了bestClusAss 中 bestClusAss[:, 0].A == 1 的数据子集.
'''
# 可以进行递归二分, k -- k-1 -- k-2 -- k-3 .... 2。
def binKMeans(dataSet, k, distMeas = distEclud):
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    ax.scatter(dataSet[:, 0], dataSet[:, 1])
    plt.show(block = False)
    patchList = []

    m = shape(dataSet)[0]
    print(m)
    # 每point分类结果信息 [所属,距离] 默认所属 都是0 质心
    clusterAssment = mat(zeros((m, 2)))
    centRoid0 = mean(dataSet, axis=0).tolist()[0]
    cenList = [centRoid0]
    # 计算所有数据点 聚类结果信息-距离
    for j in range(m):
        clusterAssment[j, 1] = distMeas(mat(centRoid0), dataSet[j, :])**2
        
    while len(cenList) < k:
        lowestSSE = inf
        # 逐个族 进行二分
        for i in range(len(cenList)):
            ptsInCurClust = dataSet[nonzero(clusterAssment[:, 0].A == i)[0], :]
            print("Current Split cluster is %d " %i)
            # 该族二分后 聚类结果, 聚类信息(- 每point[所属, 距离])
            centRoidMat, splitClusAss = kMeans(ptsInCurClust, 2, distMeas)
            #calc the SSE of 当前族 当前聚类方案的 SSE 误差平方和
            sseSplit = sum(splitClusAss[:, 1])
            #calc the SSE of 非当前族 的SSE误差平方和
            sseNotSplit = sum(clusterAssment[nonzero(clusterAssment[:, 0].A != i)[0], 1])
            # bestCentToSplit 二分后SSE最好的当前类ID
            # bestNewCents 二分聚类结果
            # bestClusAss  二分聚类结果信息
            # lowestSSE    当前的SSE 误差平方和
            print(sseSplit+sseNotSplit, lowestSSE)
            if (sseSplit + sseNotSplit) < lowestSSE:
                bestCentToSplit = i
                bestNewCents = centRoidMat
                bestClusAss = splitClusAss.copy()
                lowestSSE = sseSplit + sseNotSplit
        # bestClusAss 原bestCentToSplit 需要二分类的族数据 二分后结果信息
        # 执行二分类 将聚类结果信息bestClusAss中 
        # 分为0 类的 其类别ID 不变还用原本bestCentToSplit 类别ID
        # 分为1 类的 其类别ID 在原类别总数上增加1，作为其类别ID
        print(" >>>>>>>>>>>>>>>> ", bestCentToSplit, len(cenList))
        # ERROR 顺序不能切换 bestCentToSpllit 和 len(cenList) 不然会偶发出现全部分类给len(cenList) 这个不应该啊，不该会分类出现这个错误才是
        bestClusAss[nonzero(bestClusAss[:, 0].A == 1)[0], 0] = len(cenList)
        bestClusAss[nonzero(bestClusAss[:, 0].A == 0)[0], 0] = bestCentToSplit
        print("the bestCentToSplit is %d" %bestCentToSplit)
        print(bestNewCents)
        cenList[bestCentToSplit] = bestNewCents[0, :].tolist()[0]
        cenList.append(bestNewCents[1, :].tolist()[0])

        if len(patchList) > 0:
            for patch in patchList:
                print(patch)
                ax.patches.remove(patch)
        patchList = []
        for i in range(len(cenList)):
            circle = Circle((mat(cenList)[i, 0], mat(cenList)[i, 1]), 0.5, facecolor='none', edgecolor=(0, 0.8, 0.8), linewidth=1, alpha=0.8)
            patchList.append(circle)
            ax.add_patch(circle)
        plt.pause(0.8)
        # reassign new clusters, and SSE
        clusterAssment[nonzero(clusterAssment[:,0].A == bestCentToSplit)[0],:] \
            = bestClusAss
    plt.show()
    return mat(cenList), clusterAssment
        
        
def plotMap():
    dataMat = mat(loadDataSet('testSet.txt'))
    centList, clusterAssment = binKMeans(dataMat, 8)
    
    

def easyToUse():
    dataMat = mat(loadDataSet('testSet.txt'))
    centList, clusterAssment = binKMeans(dataMat, 4)
    centListS, clusterAssmentS = kMeans(dataMat, 4)
    print("kMeans result  .... ")
    print()
    print(centListS)
    print(centList)


if __name__ == "__main__":
    plotMap()

