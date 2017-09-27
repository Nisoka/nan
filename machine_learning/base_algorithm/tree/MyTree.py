#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd
from math import log
import operator
import copy
import re


def loadDataT():
    df=pd.read_csv('TreeData.csv')

    data=df.values[:11,1:].tolist()
    data_full=data[:]
    labels=df.columns.values[1:-1].tolist()
    labels_full=labels[:]



def loadData():
    # List Widget class [columns , values]
    dataSet = pd.read_csv('TreeData.csv')
    #data type: DateFrame
    dataIn = dataSet.values[:11,1:].tolist()
    dataTest = dataSet.values[11:, 1:].tolist()
#    print(np.array(dataIn))
#    print(np.array(dataTest))
    labels = dataSet.columns.values[1:].tolist()
    return dataIn, dataTest, labels

def calGini(dataSet):
    numEntries = len(dataSet)
    # data type: dict
    classList = {}
    for entry in dataSet:
        theClass = entry[-1]
        if theClass not in classList.keys():
            classList[theClass] = 0
        classList[theClass] += 1

    Gini = 1.0
    for key in classList:
        prob = float(classList[key])/numEntries
        Gini -= prob*prob
    return Gini

def calcEntroy(dataSet):
    numEntries = len(dataSet)
    classList = {}
    for entry in dataSet:
        theClass = entry[-1]
#        print(theClass)
        if theClass not in classList.keys():
            classList[theClass] = 0
        classList[theClass] += 1

    shannonEntroy = 0.0
    for theClass in classList:
        prob = float(classList[theClass])/numEntries
        shannonEntroy -= prob*log(prob)

    return shannonEntroy

#def splitDataSet(dataSet,axis,value):
#    retDataSet=[]
#    for featVec in dataSet:
#        if featVec[axis]==value:
#            reducedFeatVec=featVec[:axis]
#            reducedFeatVec.extend(featVec[axis+1:])
#            retDataSet.append(reducedFeatVec)
#    return retDataSet

def splitDataSet(dataSet, feature, value):
    retDataSet = []
    for entry in dataSet:
        if entry[feature] == value:
            retEntry = entry[:feature]
            retEntry.extend(entry[feature+1:])
            retDataSet.append(retEntry)
    return retDataSet

# sum probx * Entroy(dataSetChild)
def calcConditionEntroy(dataSet, featureIndex):
    numEntries = len(dataSet)
    valueList = {}

    for entry in dataSet:
        featureValue = entry[featureIndex]
        if featureValue not in valueList.keys():
            valueList[featureValue] = 0
        valueList[featureValue] += 1

    conditionEntroy = 0.0
    for featureValue in valueList:
        probx = float(valueList[featureValue])/numEntries
        childDataSet = splitDataSet(dataSet, featureIndex,  featureValue)
        conditionEntroy += probx*calcEntroy(childDataSet)
    return conditionEntroy


def calcGain(dataSet, featureIndex):
    entroy = calcEntroy(dataSet)
    condtionEntroy = calcConditionEntroy(dataSet, featureIndex)
    return entroy - condtionEntroy


def selectFeature(dataSet):
    gainList = []
    index = -1
    maxGain = 0.0
    for featureIndex in range(len(dataSet[0]) - 1):
        gain = calcGain(dataSet, featureIndex)
        if gain > maxGain:
            maxGain = gain
            index = featureIndex
    return index


def majorClass(classList):
    classCount = {}
    for entry in classList:
        if entry not in classCount.keys():
            classCount[entry] = 0
        classCount[entry] += 1

    # dict max(dict) 返回 value 最大的 key.
    return max(classCount)

def createTree(dataSet, labels):

    classList = [entry[-1] for entry in dataSet]

    if len(classList) == classList.count(classList[0]):
        return classList[0]

    if len(dataSet[0]) == 1:
        return majorClass(classList)

    # 4 theTree{label:{value:childTree,value:childTree}}
    # 1 select feature
    # 2 feature label
    # 3 feature values

    theTree = {}
    featureIndex = selectFeature(dataSet)
    theLabel = labels[featureIndex]
    theTree[theLabel] = {}
    featureList = [entry[featureIndex] for entry in dataSet]


    childLabels = copy.deepcopy(labels)
    del(childLabels[featureIndex])

    uniqueFeature = set(featureList)
    for value in uniqueFeature:
        childDataSet = splitDataSet(dataSet, featureIndex, value)
#        print(value)
#        print(childDataSet)
#        print()
        theTree[theLabel][value] = createTree(childDataSet, childLabels)

    return theTree




''' '''



def classify(inputTree, entry, labels):
    classK = 0
    feature = list(inputTree.keys())[0]
    index = labels.index(feature)
    for value in inputTree[feature].keys():
        if  value == entry[index]:
            if type(inputTree[feature][value]).__name__ == 'dict':
                childTree = copy.deepcopy(inputTree[feature][value])
                classK = classify(childTree, entry, labels)
            else:
                classK = inputTree[feature][value]

    return classK

# 遍历树查找
#
def testing(inputTree, dataSet_test, labels):
    error = 0
    classK = 0
    for entry in dataSet_test:
        classK =  classify(inputTree, entry, labels)
        if classK != entry[-1]:
            error += 1
        print(classK, entry[-1])
    return error

def majorTest(classMajor, dataSet_test):
    error = 0
    for entry in dataSet_test:
        if classMajor != entry[-1]:
            error +=1

    return error



#主目标：剪枝判断
#    子树分类错误率 如果大于 裁剪后表决错误率，那么剪枝，直接返回表决分类classk
#策略： 深度优先开始进行剪枝判断，判断后剪枝
#算法：
# 1 深度遍历当前树
#    当前树的遍历，遍历次级树
#    将 测试数据集 按树结构分割，形成次级树的测试数据集
#    裁剪次级树
# 2 对当前树，进行剪枝判断
# 3 如果剪枝， 返回表决 类标签classk，否则 返回当前树

def tailorTree(inputTree, dataSet, dataSet_test, labels):
    classList = [entry[-1] for entry in dataSet]
    feature = list(inputTree.keys())[0]
#    print(labels)
#    print(feature)
    index = labels.index(feature)
    for value in inputTree[feature].keys():
        # 深度子树剪枝
        if type(inputTree[feature][value]).__name__ == 'dict':
            childTree = copy.deepcopy(inputTree[feature][value])
            childDataSet = splitDataSet(dataSet, index, value)
            childTestDataSet = splitDataSet(dataSet_test, index, value)
            childLabels = copy.deepcopy(labels)
#            del(childLabels[index])
            tailorTree(childTree, childDataSet, childTestDataSet, childLabels)

    error = testing(inputTree, dataSet_test, labels)
    classMajor = majorClass(classList)
    errorMajor = majorTest(classMajor, dataSet_test)
    print("ERROR", error, errorMajor)
    if error >= errorMajor:
        print("tailor ")
        return classMajor
    return inputTree


def main():
    dataSet, dataSet_test, labels = loadData()
    myTree = createTree(dataSet, labels)
    testing(myTree, dataSet_test, labels)
#    for entry in dataSet:
#        print(entry)
#        print(classify(myTree, entry, labels))

    print()
    print(myTree)
    resultTree = tailorTree(myTree, dataSet, dataSet_test, labels)
    print("after tailor =----------------")
    testing(resultTree, dataSet_test, labels)
    print(resultTree)



































if __name__ == "__main__":
    main()

#{'navel':
#    {
#    'even': 0,
#    'little_sinking': {
#                        'root': {
#                                 'curl_up': 0,
#                                 'little_curl_up': {
#                                                    'color': {
#                                                              'dark_green': 1,
#                                                              'black': {
#                                                                        'texture': {
#                                                                                    'little_blur': 1,
#                                                                                    'distinct': 0
#                                                                                    }
#                                                                        }
#                                                              }
#                                                    }
#                                 }
#                        },
#     'sinking': {
#                 'color': {
#                           'dark_green': 1,
#                           'black': 1,
#                           'light_white': 0
#                           }
#                 }
#     }
#}
