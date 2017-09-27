#!/usr/bin/env python
# -*- coding: utf-8 -*-

# K 均值聚类方法
import load_mnist
from numpy import *

def loadDataSet(fileName):      #general function to parse tab -delimited floats
    numFeat = len(open(fileName).readline().split('\t'))
    print(numFeat)
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
        print(centRoids)

        # 更新每个族 的位置
        for cent in range(k):
            # 获得该族下每个点
            ptsInClust = dataSet[nonzero(clusterAssment[:, 0].A == cent)[0]]
            # 按最低维求均值(矩阵 只有两个维度 这里表示按列 - 即所有point 的对应特征求均值)
            centRoids[cent, :] = mean(ptsInClust, axis=0)
    return centRoids, clusterAssment



def binKmeans(dataSet, k, distMeas = distEclud):
    m = shape(dataSet)[0]
    # 每point [所属,距离] 默认所属 都是0 质心
    clusterAssment = mat(zeros((m, 2)))
    # ??? 直接均值mean 不就是中心了么
    '''
        axis = 0 按最低维度方向, 列方向 |
        axis = 1 按次低维度方向, 行方向 --
        print(mean(dataSet, axis = 0))
        print(centRoid0)

        因为 dataSet 是一个 matrix mxn
        直接mean 得到的是一个matrix
        [[-0.10361321  0.0543012 ]]
        通过tolist()[0] 得到列表类型
        [-0.10361321250000004, 0.05430119999999998]

        如果是个list 那么结果就完全不一样了那么不会得到一个 点向量,只会是一个数值.
        如果是个ndarray 会和 matrix 类型的结果一样 因为matrix 就是ndarray的继承.
    '''
#    print(type(dataSet))
    centRoid0 = mean(dataSet, axis = 0).tolist()[0]
#    print(mean(dataSet, axis = 0))
#    print(mean(dataSet, axis = 0).tolist())
#    print(centRoid0)
#   此时只有一个分类中心 centRoid0
    cenList = [centRoid0]
    for j in range(m):
        clusterAssment[j, 1] = distMeas(mat(centRoid0), dataSet[j, :])**2
        
    while len(cenList) < k:
        lowestSSE = inf
        # 每个族 进行二分
        for i in range(len(cenList)):
            ptsInCurClust = dataSet[nonzero(clusterAssment[:, 0].A == i)[0], :]
            # 该族二分后 聚类结果, 聚类信息(- 每point[所属, 距离])
            centRoidMat, splitClusAss = kMeans(ptsInClust, 2, distMeas)
            #calc the SSE of 当前族 当前聚类方案的 SSE 误差平方和
            sseSplit = sum(splitClusAss[:, 1])
            #calc the SSE of 非当前族 的SSE误差平方和
            sseNotSplit = sum(clusterAssment[nonzero(clusterAssment[:, 0].A != i)[0], 1])
            # bestCentToSplit 二分后SSE最好的当前类ID
            # bestNewCents 二分聚类结果
            # bestClusAss  二分聚类结果信息
            # lowestSSE    当前的SSE 误差平方和
            if (sseSplit + sseNotSplit) < lowestSSE:
                bestCentToSplit = i
                bestNewCents = centRoidMat
                bestClusAss = splitClusAss.copy()
                lowestSSE = sseSplit + sseNotSplit

        '''
        bestClusAss[:, 0]              第一列
        bestClusAss[:, 0].A            第一列变为Array类型
        bestClusAss[:, 0].A == 1       新的Array 做 == 1比较 筛选构成  true false的Array
        nonzero(bestClusAss[:, 0].A == 1)[0]
            nonzero() 返回生成的trueFalseArray中的True构成的array,内容是 [array, type]
            所以 nonzero()[0] 得到array. 描述 bestClusAss[:, 0].A == 1条件的array

        bestClusAss[array, 0]  ==> 返回了bestClusAss 中 bestClusAss[:, 0].A == 1 的数据子集.
        '''
        # bestClusAss 原bestCentToSplit 需要二分类的族数据 二分后结果信息
        # 执行二分类 将聚类结果信息中 分为0类的 其类别ID 不变还用原本bestCentToSplit
        # 分为1 类的 其类别ID 在原类别总数上增加1，作为其类别

        bestClusAss[nonzero(bestClusAss[:, 0].A == 1)[0], 0] = len(cenList)
        bestClusAss[nonzero(bestClusAss[:, 0].A == 0)[0], 0] = bestCentToSplit
        print("the bestCentToSplit is %d" %bestCentToSplit)
        cenList[bestCentToSplit] = bestNewCents[0, :].tolist()[0]
        cenList.append(bestNewCents[1, :].tolist()[0])
        #reassign new clusters, and SSE
        clusterAssment[nonzero(clusterAssment[:,0].A == bestCentToSplit)[0],:] \
            = bestClustAss    
    return mat(cenList), clusterAssment
        
        


def main():
    dataSet, labels = load_mnist.loadDataSet("trainingDigits")
    centerMat, clusterAsg = binKmeans(mat(dataSet), 10)

    

def easyToUse():
    dataMat = mat(loadDataSet('testSet.txt'))
    binKmeans(dataMat, 1)


if __name__ == "__main__":
    easyToUse()

