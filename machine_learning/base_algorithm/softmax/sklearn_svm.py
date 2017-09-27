#!/usr/bin/env python
# -*- coding: utf-8 -*-

import load_mnist
import numpy as np


from sklearn import svm
import time


def main():
    print(time.strftime('%Y-%m-%d %H-%M-%S'))
    dataSet, labels = load_mnist.loadDataSet("trainingDigits")
#    clf = svm.SVC()
    clf = svm.SVC(C=10.0, kernel='rbf', gamma=1/1000, cache_size=8000, probability=False)
    clf.fit(dataSet, labels)

    dataTestSet, labelTests = load_mnist.loadDataSet("testDigits")
    testCnt = len(labelTests)
    prediction = [int(a) for a in clf.predict(dataTestSet)]
    num_correct = sum(int(a==y) for a, y in zip(prediction, labelTests))
    print("Test correct rate is %f" % (num_correct/float(testCnt)))
    print(time.strftime('%Y-%m-%d %H-%M-%S'))


if __name__ == "__main__":
    main()

