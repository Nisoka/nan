#!/usr/bin/env python
# -*- coding: utf-8 -*-

import pickle, gzip
import numpy as np

'''
    A simple implementation of softmax regression using Numpy.
    The model is fit using gradient descent with momentum.

    Author: Matthew Green
    Date: 8/2/2016
    License: MIT
'''
class SoftmaxRegression(object):

    def __init__(self, n_cat, intercept=True, one_hot=True, rate=0.1, p=0.5, max_iter=1000):
        self.options = {'n_cat': n_cat,  # number of categories
                        'intercept': intercept,  # include intercept (bias)  包含截距项
                        'one_hot': one_hot,  # transform y with one-hot encoding    热编码，例如 0001 0010  0100 1000
                        'rate': rate,  # learning rate
                        'p': p,  # momentum coefficient
                        'max_iter': max_iter}  # max. gradient descent steps
        # self.options is a dictionaries!

    def _one_hot(self, y):
        n_cat = self.options['n_cat']
        Yb = np.zeros((y.shape[0], n_cat))
        for i in range(y.shape[0]):
            Yb[i, y[i]] = 1.0
        return Yb

    def _add_intercept(self, X):
        return np.concatenate([np.ones((X.shape[0], 1)), X], axis=1)
        # axis=1对应的是行的串联
        # array数组串联，X是n个sample*feature构成的矩阵，这句代码将每一个sample的feature都串联增加了一项 1

    def fit(self, X, y):
        '''
        Inputs:
            X := (n samples, m features) matrix
            y := (n samples, ) matrix, with entries in range 0 to n_cat-


        Notes:
            - X is transformed by adding an intercept column of ones    截距项
            if options['intercept'] is set.
            - y is transformed using one-hot encoding if options['one_hot']
            is set.
        '''
        options = self.options

        if options['intercept']:
            X_fit = self._add_intercept(X).T  # 串联1，补充截距项
        else:
            X_fit = X.T

        if options['one_hot']:
            y_fit = self._one_hot(y).T
        else:
            y_fit = y.T

        n_cat = options['n_cat']
        max_iter = options['max_iter']
        rate = options['rate']
        p = options['p']

        theta = np.zeros((n_cat, X_fit.shape[0]))
        dtheta = np.zeros(theta.shape)
        n_iter = 1
        while n_iter <= max_iter:
            z = np.dot(theta + p * dtheta, X_fit)
            zmax = np.amax(z, axis=0, keepdims=True)
            # 按列求矩阵的最大值
            ez = np.exp(z - zmax)
            a = ez / np.sum(ez, axis=0, keepdims=True)
            grad = np.dot(a - y_fit, X_fit.T)
            dtheta = p * dtheta - rate * grad
            theta += dtheta
            n_iter += 1

        self.params = theta

    def predict(self, X):
        '''
        Input:
            X := (n samples, m features) matrix
        Output:
            yhat := (n samples, ) matrix, with entries in range 0 to n_cat-1
        '''
        if self.options['intercept']:
            X_pred = self._add_intercept(X).T
        else:
            X_pred = X.T
        z = np.dot(self.params, X_pred)
        zmax = np.amax(z, axis=0, keepdims=True)
        ez = np.exp(z - zmax)
        p = ez / np.sum(ez, axis=0, keepdims=True)
        return np.argmax(p, axis=0)


def test_mnist():
    # Load the dataset
    with gzip.open('mnist.pkl.gz', 'rb') as f:
        train, valid, test = pickle.load(f, encoding='latin1')

    # 使用with结构避免手动的文件关闭操作


    import matplotlib.cm as cm
    import matplotlib.pyplot as plt
    #
    print(type(train[0].shape))
    m1 = train[0]
    plt.imshow(m1[0, ].reshape(28, 28), cmap=cm.Greys_r)
    plt.show()
    #
    ## Fit a softmax model with default options
    smr = SoftmaxRegression(n_cat=10)
    smr.fit(train[0], train[1])

    ## Evaluate test set error
    yhat = smr.predict(test[0])
    print(np.mean(yhat == test[1]))  # 0.9203


if __name__ == '__main__':
    test_mnist()
