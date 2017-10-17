#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import pandas as pd
from math import log
import operator
import copy
import re



def load_data():
    # List Widget class [columns , values]
    data = pd.read_csv('TreeData.csv')
    #data type: DateFrame
    data_train = data.values[:11,1:].tolist()
    data_test = data.values[11:, 1:].tolist()

    #dataSet.columns -- csv标签名(表头)
    feature_names = data.columns.values[1:].tolist()
    return data_train, data_test, feature_names

def calGini(dataSet):
    entry_cnt = len(dataSet)
    # data type: dict
    class_cnt_list = {}
    for entry in dataSet:
        theClass = entry[-1]
        if theClass not in class_cnt_list.keys():
            class_cnt_list[theClass] = 0
        class_cnt_list[theClass] += 1

    # GINI = -SUM{prob^2} ???
    Gini = 1.0
    for key in class_cnt_list:
        prob = float(class_cnt_list[key])/entry_cnt
        Gini -= prob*prob
    return Gini

def calcEntroy(dataSet):
    entry_cnt = len(dataSet)
    class_cnt_list = {}

    for entry in dataSet:
        theClass = entry[-1]
        if theClass not in class_cnt_list.keys():
            class_cnt_list[theClass] = 0
        class_cnt_list[theClass] += 1

    shannonEntroy = 0.0
    for theClass in class_cnt_list:
        prob = float(class_cnt_list[theClass])/entry_cnt
        shannonEntroy -= prob*log(prob)

    return shannonEntroy

def extract_data_set(dataSet, feature, value):
    retDataSet = []
    for entry in dataSet:
        if entry[feature] == value:
            retEntry = entry[:feature]
            retEntry.extend(entry[feature+1:])
            retDataSet.append(retEntry)
    return retDataSet

#conditionEntroy
def calcConditionEntroy(dataSet, featureIndex):
    entry_cnt = len(dataSet)
    value_cnt_list = {}

    for entry in dataSet:
        featureValue = entry[featureIndex]
        if featureValue not in value_cnt_list.keys():
            value_cnt_list[featureValue] = 0
        value_cnt_list[featureValue] += 1

    conditionEntroy = 0.0
    for featureValue in value_cnt_list:
        probx = float(value_cnt_list[featureValue])/entry_cnt
        childDataSet = extract_data_set(dataSet, featureIndex,  featureValue)
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

def createTree(dataSet, feature_names):

    classList = [entry[-1] for entry in dataSet]

    #only one class leave, reach the end of the tree
    if len(classList) == classList.count(classList[0]):
        return classList[0]

    #only one feature leave, no need to split any more;
    if len(dataSet[0]) == 1:
        return majorClass(classList)

    # 4 theTree{feature: {value:childTree, value:childTree}}
    # 1 select feature
    # 2 feature label
    # 3 feature values

    theTree = {}
    # use entroy to select the best feature
    featureIndex = selectFeature(dataSet)
    feature_name = feature_names[featureIndex]
    theTree[feature_name] = {}
    feature_value_list = [entry[featureIndex] for entry in dataSet]


    childfeature_names = copy.deepcopy(feature_names)
    del(childfeature_names[featureIndex])

    feature_value_set = set(feature_value_list)
    for value in feature_value_set:
        childDataSet = extract_data_set(dataSet, featureIndex, value)
        theTree[feature_name][value] = createTree(childDataSet, childfeature_names)

    return theTree

''' '''

def classify(inputTree, entry, feature_names):
    classK = 0
    feature = list(inputTree.keys())[0]
    index = feature_names.index(feature)

    for value in inputTree[feature].keys():
        if  value == entry[index]:
            if type(inputTree[feature][value]).__name__ == 'dict':
                childTree = copy.deepcopy(inputTree[feature][value])
                classK = classify(childTree, entry, feature_names)
            else:
                classK = inputTree[feature][value]

    return classK

# 遍历树查找
def testing(inputTree, dataSet_test, feature_names):
    error = 0
    classK = 0
    for entry in dataSet_test:
        classK =  classify(inputTree, entry, feature_names)
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

def tailorTree(inputTree, dataSet, dataSet_test, feature_names):
    classList = [entry[-1] for entry in dataSet]
    feature = list(inputTree.keys())[0]
#    print(feature_names)
#    print(feature)

    #递归剪枝
    index = feature_names.index(feature)
    for value in inputTree[feature].keys():
        # 深度子树剪枝
        if type(inputTree[feature][value]).__name__ == 'dict':
            childTree = copy.deepcopy(inputTree[feature][value])
            childDataSet = extract_data_set(dataSet, index, value)
            childTestDataSet = extract_data_set(dataSet_test, index, value)
            childfeature_names = copy.deepcopy(feature_names)
#            del(childfeature_names[index])
            tailorTree(childTree, childDataSet, childTestDataSet, childfeature_names)

    #使用子树进行测试错误率
    error = testing(inputTree, dataSet_test, feature_names)
    #使用majorclass 作为子数据集的class 计算错误率
    classMajor = majorClass(classList)
    errorMajor = majorTest(classMajor, dataSet_test)

    print("ERROR", error, errorMajor)
    if error >= errorMajor:
        print("tailor ")
        return classMajor
    return inputTree


def main():
    dataSet, dataSet_test, feature_names = loadData()
    myTree = createTree(dataSet, feature_names)
    testing(myTree, dataSet_test, feature_names)
#    for entry in dataSet:
#        print(entry)
#        print(classify(myTree, entry, feature_names))

    print()
    print(myTree)
    resultTree = tailorTree(myTree, dataSet, dataSet_test, feature_names)
    print("after tailor =----------------")
    testing(resultTree, dataSet_test, feature_names)
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
