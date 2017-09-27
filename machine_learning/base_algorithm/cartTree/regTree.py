#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *

'''
    [float(entry) for entry in curLine]
'''
def loadDataSet(filename):
    dataMat = []
    fr = open(filename)
    for line in fr.readlines():
        curLine = line.strip().split('\t')
        fltLine = [float(entry) for entry in curLine]
        dataMat.append(fltLine)
    return dataMat

'''
    list索引方法
    dataSet[ nonzero( dataSet[:,feature] > value )[0] , : ]
'''
def binSplitDataSet(dataSet, feature, value):
#    print(dataSet[nonzero(dataSet[:,feature] > value)[0],:][0])
#    print("========================")
    mat0 = dataSet[nonzero(dataSet[:,feature] > value)[0],:]
    mat1 = dataSet[nonzero(dataSet[:,feature] <= value)[0],:]
    return mat0,mat1

'''
    np
    1 均值
        mean( ndarray )
    2 方差
        var( ndarray )
    3 求逆
'''
def regLeaf(dataSet):
    # y 的均值
    return mean(dataSet[:, -1])

def regErr(dataSet):
    return var(dataSet[:, -1]) * shape(dataSet)[0]

def linearSolve(dataSet):
    m, n = shape(dataSet)
    X = mat(ones((m, n)))
    Y = mat(ones((m, 1)))
    # 直接赋值
    X[:, 1:n] = dataSet[:, 0:n-1]
    Y = dataSet[:, -1]

    xTx = X.T*X
    if linalg.det(xTx) == 0.0:
        raise NameError("This matrix is singular, cannot do inverse \n")
    ws = xTx.I * (X.T * Y)
    return ws, X, Y

def modelLeaf(dataSet):
    ws, X, Y = linearSolve(dataSet)
    return ws

def modelErr(dataSet):
    ws, X, Y = linearSolve(dataSet)
    yHat = X * ws
    return sum(power(Y - yHat, 2))






'''
    这里定义的ops 是一个预剪枝处理, 人为提供了停止条件
然而，通过不断修改停止条件来得到合理结果并不是很好的办法。这样人为提供的修剪处理容易带来问题,
事实上，我们常常甚至不确定到底需要寻找什么样的结果。
这正是机器学习所关注的内容，计算机应该可以给出总体的概貌。
下节将讨论后剪枝，即利用测试集来对树进行剪枝。由于不需要用户指定参数，后剪枝是一个更理想化的剪枝方法。

'''
def isTree(obj):
    return (type(obj).__name__ == 'dict')

def getMean(tree):
    if isTree(tree['right']):
        tree['right'] = getMean(tree['right'])
    if isTree(tree['left']):
        tree['left'] = getMean(tree['left'])

    return (tree['left'] + tree['right'])/2.0


def prune(tree, testData):
    if shape(testData)[0] == 0:
        return getMean(tree)
    if isTree(tree['right']) or isTree(tree['left']):
        lSet, rSet = binSplitDataSet(testData, tree['spInd'], tree['spVal'])
    if isTree(tree['left']):
        tree['left'] = prune(tree['left'], lSet)
    if isTree(tree['right']):
        tree['right'] = prune(tree['right'], rSet)

    if not isTree(tree['left']) and not isTree(tree['right']):
        lSet, rSet = binSplitDataSet(testData, tree['spInd'], tree['spVal'])
        errorNoMerge = sum(power(lSet[:,-1] - tree['left'], 2)) + \
                        sum(power(rSet[:,-1] - tree['right'], 2))
        treeMean = (tree['left'] + tree['right'])/2.0
        errorMerge = sum(power(testData[:,-1] - treeMean, 2))
        if errorMerge < errorNoMerge:
            print("merging")
            return treeMean
        else:
            return tree
    else:
        return tree




# 找到最佳二元分类 feature value
'''
    三种情况不再切分,
        1 y值 都相同 不需要再进行切分时, 形成叶子节点返回.
        2 S - bestS 切分前后误差减少不大, 不用再切分 形成叶子节点返回.
        3 切分前后 有一个数据子集内容太少, 没有必要这么切分 增加过拟合风险.形成叶子节点返回.
        其中变量tolS是容许的误差下降值，tolN是切分的最少样本数。

'''
def chooseBestFeat2Split(dataSet, leafFunc = regLeaf, errFunc = regErr, ops = (1, 4)):
    tolS = ops[0]
    tolN = ops[1]
    if len(set(dataSet[:,-1].T.tolist()[0])) == 1:
        return None, leafFunc(dataSet)
    m, n = shape(dataSet)
#    print(m, n)
#    print(dataSet)
    S = errFunc(dataSet)
#    print(S)
    bestS = inf
    bestIndex = 0
    bestValue = 0
    for featIndex in range(n-1):
        for splitVal in set([entry[0,featIndex] for entry in dataSet]):
            mat0, mat1 = binSplitDataSet(dataSet, featIndex, splitVal)
            if shape(mat0)[0] < tolN or shape(mat1)[0] < tolN:
                continue
            newS = errFunc(mat0) + errFunc(mat1)
            if newS < bestS:
                bestIndex = featIndex
                bestValue = splitVal
                bestS = newS

    # 数据集误差减少很小, 说明最大切分方式也不能很有效切分数据了, 直接形成叶子节点.
    if S - bestS < tolS:
        return None, leafFunc(dataSet)

    # 切分数据集 有一个很小, 就不对其进行分割, 直接形成叶子节点.
    mat0, mat1 = binSplitDataSet(dataSet, bestIndex, bestValue)
    if (shape(mat0)[0] < tolN) or (shape(mat1)[0] < tolN):
        return None, leafFunc(dataSet)

    return bestIndex, bestValue



def createTree(dataSet, leafFunc = regLeaf, errFunc = regErr, ops = (1, 4)):
    feat, val = chooseBestFeat2Split(dataSet, leafFunc, errFunc, ops)
    if feat == None:
        return val
    retTree = {}
    retTree['spInd'] = feat
    retTree['spVal'] = val
    lSet, rSet = binSplitDataSet(dataSet, feat, val)
    retTree['left'] = createTree(lSet, leafFunc, errFunc, ops)
    retTree['right'] = createTree(rSet, leafFunc, errFunc, ops)
    return retTree


def regTreeEval(model, inDat):
    return float(model)

def modelTreeEval(model, inDat):
    n = shape(inDat)[1]
    xVec = mat(ones((1, n+1)))
    xVec[:, 1:n+1] = inDat
    return float(xVec*model)

def treeForeCast(tree, inData, modelEval = regTreeEval):
    # Stop condition: notTree
    # can't switch anymore.
    if not isTree(tree):
        return modelEval(tree, inData)

    if inData[tree['spInd']] > tree['spVal']:
        if isTree(tree['left']):
            return treeForeCast(tree['left'], inData, modelEval)
        else:
            return modelEval(tree['left'], inData)
    else:
        if isTree(tree['right']):
            return treeForeCast(tree['right'], inData, modelEval)
        else:
            return modelEval(tree['right'], inData)

def createForeCast(tree, testData, modelEval = regTreeEval):
    m = len(testData)
    yHat = mat(zeros((m, 1)))
    for i in range(m):
        yHat[i, 0] = treeForeCast(tree, mat(testData[i]), modelEval)
    return yHat



def main():
#    myData = loadDataSet('ex00.txt')
#    myMat = mat(myData)
#    myTree = createTree(myMat)
#    print(myTree)

#    myData = loadDataSet('exp2.txt')
#    myMat = mat(myData)
#    myTree = createTree(myMat, modelLeaf, modelErr, (1, 10))
#    print(myTree)
#    print(" -------------------- ")
#    testData = loadDataSet('ex2test.txt')
#    myTest = mat(testData)
#    pruneTree = prune(myTree, myTest)
#    print(pruneTree)

    myData = loadDataSet('bikeSpeedVsIq_train.txt')
    myMat = mat(myData)
    testData = loadDataSet('bikeSpeedVsIq_test.txt')
    testMat = mat(testData)
    myTree = createTree(myMat, ops = (1, 20))
    yHat = createForeCast(myTree, testMat[:, 0])
    coef = corrcoef(yHat, testMat[:, 1], rowvar=0)[0, 1]
    print(myTree)
    print(coef)
    print(" -------------------- ")

    myTree = createTree(myMat, modelLeaf, modelErr, ops = (1, 20))
    yHat = createForeCast(myTree, testMat[:, 0], modelTreeEval)
    coef = corrcoef(yHat, testMat[:, 1], rowvar=0)[0, 1]
    print(myTree)
    print(coef)
    print(" -------------------- ")


if __name__ == "__main__":
    main()
