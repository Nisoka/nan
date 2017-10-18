# -*- coding: utf-8 -*-

import scipy as sp
import numpy as np
import random

# loadData
def loadData_tsv(fileName):
    '''
    input fileName
    return data list
    '''
    fr = open(fileName)
    data = []
    labels = []
    for line in fr.readlines():
        lineArr = line.strip().split('\t')
        data.append(lineArr[:-1])
        labels.append(lineArr[-1])
    return data, labels

# splitData to trian and test
def splitDataIndex(dataCnt, rate):
    '''
    return trainIndex, testIndex (list type)
    '''
    trainIndex = list(range(dataCnt))
    # sample the listIndex
    testIndex = random.sample(trainIndex, int(dataCnt*rate))
    for value in testIndex:
        trainIndex.remove(value)

    return trainIndex, testIndex



# preprocess
def preprocess_nan_y_del(x, y):
    x = x[~sp.isnan(y)]
    y = y[~sp.isnan(y)]
    return x, y

# z normalize( mean = 0, width = value/biaozhuncha)
def preprecess_norm_z(features):
    features -= features.mean(axis = 0)
    # features /= biaozuncha
    features /= features.std(axis = 0)
    return features

# error funcs
def error_square(predict, x, y):
    return sp.sum( (predict(x) - y)**2 )



# plot funcs
def plot_func(func, x):
    fx = sp.linspace(0, x[-1], 1000)
    plt.plot(fx, func(fx), linewidth = 3)
    plt.legend(["d = %i" % func.order], loc="upper left")

