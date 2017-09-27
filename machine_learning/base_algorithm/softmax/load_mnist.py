#!/usr/bin/env python
# -*- coding: utf-8 -*-

from os import listdir

def img2vector(fileName):
    retList = []
    fr = open(fileName)
    for line in fr.readlines():
        line = line[:-1]
        lineI = map(int, line)
        retList.extend(lineI)

    return retList

def loadDataSet(dirName):
    fileList = listdir(dirName)
    m = len(fileList)
    dataSet = []
    labels = []
    for i in range(m):
        fileName = fileList[i]
        nameStr = fileName.split('.')[0]
        label = int(nameStr.split('_')[0])
        labels.append(label)
        dataSet.append(img2vector('%s/%s' %(dirName, fileName)))
#    print(sum(dataSet[1]))
    return dataSet, labels
