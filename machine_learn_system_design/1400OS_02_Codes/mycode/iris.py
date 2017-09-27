#!/usr/bin/env python
# -*- coding: utf-8 -*-


from matplotlib import pyplot as plt
from sklearn.datasets import load_iris
import numpy as np

# target = 0, 1, 2 ( setosa, virginica, versicolour)
def loadData():
    data = load_iris()
    features = data['data']
    feature_names = data['feature_names']
    target = data['target']

    return data, features, feature_names, target

# step 2  可视化 直观理解数据
def plotData(data):
    features = data['data']
    target = data['target']

    for t, marker, c in zip(range(3), '>ox', 'rgb'):
        plt.scatter(features[target == t, 0],
                    features[target == t, 1],
                    marker = marker,
                    c = c)

def printData(data):
    features = data['data']
    feature_names = data['feature_names']
    target = data['target']
    print(features)
    print(feature_names)
    print(target)

# step 4 选择分类模型 first classify, Threshold
def firstClassify(data):
    features = data['data']
    feature_names = data['feature_names']
    target = data['target']

    plength = features[:, 2]
    # 判断 得到 bool ndarray(条件数组), to index the ndarray
    is_setosa = (target == 0)
    # ndarray 的条件索引
    max_setosa = plength[is_setosa].max()
    min_non_setosa = plength[~is_setosa].min()
    print("setosa max p length : ", max_setosa)
    print("non setosa min p length : ", min_non_setosa)

#    for i in range(features[:, 2].shape[0]):
#        if features[i, 2] < 2:
#            print("Iris Setosa")

    features = features[~is_setosa]
    target = target[~is_setosa]
    # bool ndarray, to index the ndarray
    is_virginica = (target == 1)
    print(is_virginica)
    best_acc = -1.0
    cmpDir = ['lf', 'rg']
    for fi in range(features.shape[1]):
        # copy the ndarray
        thresh = features[:, fi].copy()
        # sort the ndarray
        thresh.sort()
        for t in thresh:
            for dir in cmpDir:
                if dir == 'lf':
                    pred = (features[:, fi] < t)
                else:
                    pred = (features[:, fi] > t)
                # calc the mean
                acc = (pred == is_virginica).mean()
                if acc > best_acc:
                    best_dir = dir
                    best_acc = acc
                    best_fi = fi
                    best_t = t
                    best_pred = pred
#    print(best_acc)
#    print(best_pred)
    for i in range(features.shape[0]):
        if best_dir == 'lf':
            if features[i, best_fi] < best_t:
                print(" Iris Virginica")
            else:
                print(" Other ")
        else:
            if features[i, best_fi] > best_t:
                print(" Iris Virginica")
            else:
                print(" Other ")


def easyToUse():
    data, features, feature_names, target = loadData()
#    printData(data)
#    plotData(data)
    firstClassify(data)

    print("before show plt")
#    plt.show()


if __name__ == "__main__":
    easyToUse()
