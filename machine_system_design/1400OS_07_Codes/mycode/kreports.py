#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from sklearn.datasets import load_svmlight_file
from sklearn.linear_model import LinearRegression
DATA_DIR = r"../data/E2006.train"

def loadData():
    data, target = load_svmlight_file(DATA_DIR)
    print('Min target value: {}'.format(target.min()))
    print('Max target value: {}'.format(target.max()))
    print('Mean target value: {}'.format(target.mean()))
    print('std target value: {}'.format(target.std()))
    return data, target

def linearReg(data, target):
    lr = LinearRegression(fit_intercept=True)
    lr.fit(data, target)
    p = np.array(map(lr.predict, data))
    p = p.ravel()
    e = p - target
    total_sq_error = np.sum(e**2)
    rmse_train = np.sqrt(total_sq_error/len(p))
    print(rmse_train)


if __name__ == "__main__":
    data, target = loadData()
    linearReg(data, target)

