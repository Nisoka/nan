#!/usr/bin/env python
# -*- coding: utf-8 -*-

# feature engineering!!!
# good feature: important has more different
# usualy you may use perfessional infomation to create a good feature.

# feature selection!!!
# select some good feature , and drop others bad feature may have good benift

import random
import numpy as np
import sys
sys.path.insert(0, "../../")
from datafunc import datafunc as dfc
from mathfunc import mathfunc as mfc
from typefunc import typefunc as tfc


def distance(p0, p1):
    'Computes squred euclidean distance'
    return np.sum( (p0 - p1)**2 )

# nn may be use weight because some times some feature is more important than others
def nn_classify(training_set, training_labels, new_example):
    dists = np.array([mfc.distance_euc(t, new_example) for t in training_set])
    # argmin()  return the index of Min in ndarray
    nearest = dists.argmin()
    return training_labels[nearest]

def easyToUse():
    help(dfc.loadData_tsv)
    data, labels = dfc.loadData_tsv('seeds.tsv')
    testStart = int(2*len(data)/3)

    data = np.array(data, dtype = 'float32')
    data = dfc.preprecess_norm_z(data)
    labels = np.array(labels)

    trainIndex, testIndex = dfc.splitDataIndex(len(data), 0.3)
    train_data = data[trainIndex]
    test_data = data[testIndex]
    train_labels = labels[trainIndex]
    test_labels = labels[testIndex]

    errCnt = 0
    testCnt = len(test_data)
    for i in range(testCnt):
        example = test_data[i]
        label = nn_classify(train_data, train_labels, example)
        if label != test_labels[i]:
            errCnt += 1

    print("nn predict right rate is" ,1 - errCnt/float(testCnt))



if __name__ == "__main__":
    easyToUse()
