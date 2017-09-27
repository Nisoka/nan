#!/usr/bin/env python
# -*- coding: utf-8 -*-

from math import log
import pandas as pd
import numpy as np
import operator
import copy
import re

'''
csv读取之后 得到的是 pandas csv 对象 内容包括 columns values
    [columns]   标签
    [values]    数据
'''
def loadData():
    dataSet = pd.read_csv('TreeData.csv')
    dataIn = dataSet.values[:11, 1:].tolist()
    dataTest = dataSet.values[11:, 1:].tolist()
    labels = dataSet.columns.values[1:].tolist()
    print(dataIn)
    print(type(dataIn))
    print(mat(dataIn))
    print(type(mat(dataIn)))
    return dataIn, dataTest, labels



def loadData2():
    fr = open('lenses.txt')
    dataSet = []
    for instance in fr.readlines():
        lineArr = instance.strip().split('\t')
        dataSet.append(lineArr)
    labels = ['age', 'prescript', 'astigmatic', 'testRate']
    return dataSet, labels
'''
    简介
    第2章介绍的k近邻算法可以完成很多分类任务，但是它最大的缺点就是无法给出数据的内
在含义。
    <决策树的主要优势就在于学习到的模型规则 非常容易理解>
    决策树很多任务都是 从数据中学习总结知识信息，因此决策树可以使用不熟悉的数据集合.
    从中提取出一些列规则, 学习提取到的规则,就是决策树算法学习到的模型.
    专家系统中经常使用决策树算法。而经过决策树算法的规则得出的结果, 往往能够匹敌专家给出的结论。

    1 构造决策树的方法
    2 度量算法正确率的方法
    3 建立分类器。

    3.1 构建决策树
    优点:
        计算复杂度不高, 输出模型易于理解, 对中间值缺失不敏感, 可以处理不相关数据
    缺点:
        容易产生过拟合, 可以使用剪枝处理 剪去过拟合.
    适用数据类型:
        标称型, 数值型
        ID3 适用于标称型数据, 而不适用与数值型。

    1 数学上 使用信息论 度量数据集的信息量  --- 香农信息熵
        使用信息论的 熵概念 划分数据集, 有效降低数据集复杂度的理论。

    2 有效划分数据集
        划分大原则是：将无序的数据变得更加有序,组织杂乱无章数据的一种方法就是使用信息论度量信息.
        划分数据前后 信息发生的变化 称为信息增益.
        知道如何计算信息增益  计算按每个特征划分的信息增益, 找到最大的信息增益即可.
    3 通过度量数据集信息、有效划分数据集, 递归创建树
        1 应用信息论 - 信息熵 - 信息增益 原理
        2 找到最好能够划分数据集的特征, 这就需要评估每个特征(遍历).
        3 按该特征划分数据集为多个数据子集.
            然后判断该数据集是否已经完好划分,
                如果完好划分每个子集都属于同一类别, 则决策树构建结束.
                如果还有数据子集不属于同一类别,即还需要划分, 递归执行.

    3.2 matplotlib 绘制树


    3.3 测试和存储分类器 classifyer()

    1 使用决策树 构建分类器
        执行数据分类时, 需要构建好的决策树,以及数据标签,
        因为决策树 是通过特征标签描述的.
        也是一个递归函数。
    2 使用算法 决策树的存储
        使用创建好的决策树进行决策 是很简单的事情。
        决策树 保存下来在读取出来 就可以使用

        通过将对象执行序列化可以保存对象。并在需要的时候可以读取出来。任何对象都可以执行序列化操作.


'''

def calcShannonEnt(dataSet):
    numEntries = len(dataSet)
    # 统计各个类别包含样例数
    labelCounts = {}
    for featVec in dataSet:
        currentLabel = featVec[-1]
        if currentLabel not in labelCounts.keys():
            labelCounts[currentLabel] = 0
        labelCounts[currentLabel] += 1

    # 计算数据集香农熵, sum{ -prob*log(p) }
    #　香农熵 = - 所有分类k{概率p*log(概率P)}
    #  数据越杂乱(分类越多), 熵越大
    shannonEnt = 0.0
    for key in labelCounts:
        prob = float(labelCounts[key])/numEntries
        shannonEnt -= prob*log(prob, 2)

    return shannonEnt

'''
list 自带了extend append 两个方法函数, 这两个方法功能类似,
但是在处理多个列表时，这两个方法的处理结果是完全不同的
a.append(b)
a.extend(b)
append 方法 将 b 整体作为元素 添加到a里。
    就是append 的常见用法
extend 方法 判断如果b 是一个list 则把b中的元素 添加到a里。
    extend 方法 不应该叫添加, 应该叫扩展.
'''
def splitDataSet(dataSet, axis, value):
    retDataSet = []
    for featVec in dataSet:
        if featVec[axis] == value:
            reduceFeatVec = featVec[:axis]
            reduceFeatVec.extend(featVec[axis+1:])
            retDataSet.append(reduceFeatVec)
    return retDataSet

# 有效的划分数据集
def chooseBestFeatureToSplit(dataSet):
    numFeatures = len(dataSet[0]) - 1
    baseEntropy = calcShannonEnt(dataSet)
    bestInfoGain = 0.0
    bestFeature = -1
    # 遍历所有特征
    for i in range(numFeatures):
        featValueList = [example[i] for example in dataSet]
        # 特征素有可能值 生成set
        uniqueVals = set(featValueList)
        newEntropy = 0.0
        # 按所有可能值 划分数据集, 计算新的 数据子集总体香农熵.
        # 因为对数据集进行了划分, 所以整体信息变少了.
        for value in uniqueVals:
            subDataSet = splitDataSet(dataSet, i, value)
            prob = len(subDataSet)/float(len(dataSet))
            newEntropy += prob*calcShannonEnt(subDataSet)
        # 通过遍历所有特征, 找到最大的信息增益.
        infoGain = baseEntropy - newEntropy
        if infoGain > bestInfoGain:
            bestInfoGain = infoGain
            bestFeature = i
    return bestFeature

# 票决方式决定所属分类
def majorCnt(classList):
    classCount = {}
    for vote in classList:
        if vote not in classCount.keys():
            classCount[vote] = 0
        classCount[vote] += 1
    sortedClassCount = sorted(classCount.items(), key=operator.itemgetter(1), reverse=True)
    # {key0: value0, key1:value1}
    # 经过sorted排序 按照 itemgetter(1) - value 从大到小排序.
    return sortedClassCount[0][0]

'''
变量myTree 包含了很多代表树结构信息的嵌套字典，从左边开始，
第一个关键字no surfaCing是第一个划分数据集的特征名称，该关键字的值也是另一个数据字典。第二个关键字
第二个关键字是no surfacing特征划分的数据集，这些关键字的值是no surfacing节点的子节点。
    这些值可能是类标签，也可能是另一个数据字典。
    如果值是类标签，则该子节点是叶子节点；
    如果值是另一个数据字典，则子节点是一个判断节点，这种格式结构不断重复就构成了整棵树。
    本节的例子中，这棵树包含了3个叶子节点以及2个判断节点
    {feature1:{ feat1value1: class, feat1value2:{ feature2:{ feat2value1: class, feat2value2: class}}}}
     key         key         value    key          key         key         value  key         value
               value                             value      value
'''
def createTree(dataSet, labels):
    classList = [example[-1] for example in dataSet]
    if classList.count(classList[0]) == len(dataSet):
        return classList[0]
    # 每个entry 只有一个元素 - label , 没有特征了, 进行票决
    if len(dataSet[0]) == 1:
        return majorCnt(classList)

    bestFeat = chooseBestFeatureToSplit(dataSet)
    bestFeatLabel = labels[bestFeat]
    myTree = {bestFeatLabel:{}}
    childLabels = copy.deepcopy(labels)
    del(childLabels[bestFeat])
    featValues = [example[bestFeat] for example in dataSet]
    uniqueVals = set(featValues)
    for value in uniqueVals:
        myTree[bestFeatLabel][value] = createTree(splitDataSet(dataSet, bestFeat, value), childLabels)
    return myTree

#分类算法
def classify(inputTree, entry, labels):
    classK = 0
    feature = list(inputTree.keys())[0]
    #print(feature)
    index = labels.index(feature)
    for value in inputTree[feature].keys():
        if value == entry[index]:
            #print(value)
            if type(inputTree[feature][value]).__name__ == 'dict':
                childTree = copy.deepcopy(inputTree[feature][value])
                classK = classify(childTree, entry, labels)
            else:
                classK = inputTree[feature][value]
    return classK

def testData(inputTree, dataTest, labels):
    classK = -1
    for entry in dataTest:
        print(">>>>>")
        classK = classify(inputTree, entry, labels)
        print(classK, entry[-1])

def createDataSet():
    dataSet = [[1, 1, 'yes'],
               [1, 1, 'yes'],
               [1, 0, 'no'],
               [0, 1, 'no'],
               [0, 0, 'no']]
    labels = ['no surfacing', 'flippers']
    return dataSet, labels

def createDataSet2():
    dataSet = [[1, 1, 'maybe'],
               [1, 1, 'yes'],
               [1, 0, 'no'],
               [0, 1, 'no'],
               [0, 0, 'no']]
    labels = ['no surfacing', 'flippers']
    return dataSet, labels

'''
因为 书本上是 python2 这里使用python 3
这里pickle.dump 要求open(filename, 'wb')
这里pickle.load 要求open(filename, 'rb')
'''
# 通过 pickle dump 直接将对象按结构保存
def storeTree(inputTree, filename):
    import pickle
    fw = open(filename, 'wb')
    pickle.dump(inputTree, fw)
    fw.close()

# 通过 pickle load 将保存的对象按结构读取 直接生成原本
def grabTree(filename):
    import pickle
    fr = open(filename, 'rb')
    return pickle.load(fr)

def easyToUse():
#    dataSet, labels = createDataSet()
#    shannonEnt = calcShannonEnt(dataSet)
#    print(shannonEnt)

    dataIn, dataTest, labels = loadData()
#    myTree = createTree(dataIn, labels)
#    testData(myTree, dataTest, labels)

#    dataSet, labels = loadData2()
#    myTree = createTree(dataSet, labels)
#    print(myTree)
#    storeTree(myTree, 'myTree.txt')
#    loadTree = grabTree('myTree.txt')




if __name__ == "__main__":
    easyToUse()
