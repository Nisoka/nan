#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from sklearn.datasets import load_boston
from sklearn.linear_model import LinearRegression
from sklearn.cross_validation import KFold
from matplotlib import pyplot as plt

def boston_loadData():
    boston = load_boston()
    return boston



def easyToUse():
    boston = boston_loadData()
    plt.scatter(boston.data[:, 5], boston.target, color = 'r')
    x = boston.data[:, 5]
    x = np.array([[v, 1] for v in x])
    y = boston.target
    (slope,bias), total_error, _, _ = np.linalg.lstsq(x, y)
    w = np.array([slope, bias])
    yHat = np.dot(w, x.T) # [1x2] X [2xm] = [1xm]
    plt.plot(x[:,0], yHat, color = 'b')
    rmse = np.sqrt(total_error[0]/len(x))
    print(rmse, total_error[0])


    num_samples = len(boston.data)
    add_x = np.array(np.ones((num_samples, 1)), dtype='float32')
    x = np.concatenate((boston.data, add_x), axis=1)
#    print(num_samples)
#    y = boston.target
    w, total_error, _, _ = np.linalg.lstsq(x, y)
    rmse = np.sqrt(total_error[0]/len(x))
    print(rmse, total_error[0])

#    plt.show()

def skLinearRegression():
    boston = boston_loadData()
    plt.scatter(boston.data[:, 5], boston.target, color = 'r')
    x = boston.data
    y = boston.target
    lr = LinearRegression(fit_intercept=True)
    lr.fit(x, y)

    def predict(x):
        x = x.reshape(1, -1)
        return lr.predict(x)[0]

    p = list(map(predict, x))
    error = p-y
    total_error = np.sum(error**2)
    rmse_train = np.sqrt(total_error/len(x))
    print(rmse_train)

    kf = KFold(len(x), n_folds = 10)
    err = 0
    for train, test in kf:
        lr.fit(x[train], y[train])
        p = list(map(predict, x[test]))
        e = p - y[test]
        err += np.sum(e**2)

    rmse_10cv = np.sqrt(err/len(x))
    print(rmse_10cv)

def testf(x):
    return x**2
def test():
    x = [1, 2, 3, 4]
    print(list(map(testf, x)))



if __name__ == "__main__":
    skLinearRegression()
    test()

