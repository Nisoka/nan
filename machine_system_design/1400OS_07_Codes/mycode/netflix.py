#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
from scipy import sparse
from sklearn.linear_model import LassoCV, RidgeCV, ElasticNetCV
from sklearn.cross_validation import KFold
alphasGroup = [0.125, 0.25, 0.5, 1, 2, 4]
regLasso = LassoCV(fit_intercept = True, alphas = alphasGroup)
regElast = ElasticNetCV(fit_intercept=True, alphas = alphasGroup)

def loadData():
    data = [ [int(tok) for tok in line.strip().split('\t')[:3]] for line in open("../data/ml-100k/u.data")]
    return np.array(data)

def preprocessData(data):
    ij = data[:,:2] # [user, movie?]
    ij -= 1
    values = data[:, 2] #[value]
    reviews = sparse.csc_matrix((values,ij.T)).astype(float)  # Row:user, Col:movie  value: values
    return reviews


data = loadData()
reviews = preprocessData(data)

def movie_norm(x, options = 'before'):
    # xc movieCnt x userCnt
    xc = x.copy().toarray()
    # nan 2 0
    if options == 'before':
        xc = np.nan_to_num(xc)
    # movie mean value
    x1 = np.array([xi[xi > 0].mean() for xi in xc])

    if options != 'before':
        x1 = np.nan_to_num(x1)

    # for i in range(movieCnt)
    for i in range(xc.shape[0]):
        # norm the score - mean.
        #(那那些评分为0的值呢 ？这样均值的数据 和 为0的数据一样了)
        # 评分为0 实际上是未评分, 所以直接认为是均值mean 就可以了.
        xc[i] -= (xc[i] > 0) * x1[i]
    return xc, x1



def netflixRegPredict(regModel, user_id = 0, normalizeOptions = 'before'):
    i = user_id
    u = reviews[i]
    u = u.toarray().ravel()
    ps, = np.where(u > 0)
    # Other Users
    us = np.delete(np.arange(reviews.shape[0]), i)
    # Other users and user_i same movies
    x = reviews[us][:,ps].T
    # Other users target is user_i' value
    y = u[ps]

    def reg_predict(x):
        x = x.reshape( (1, len(x)) )
        return regModel.predict(x)

    err = 0
    eMean = 0
    kf = KFold(len(y), n_folds=4)
    for train_indices, test_indices in kf:
        xc, x1 = movie_norm(x[train_indices], normalizeOptions)
        regModel.fit(xc, y[train_indices] - x1)

        xc, x1 = movie_norm(x[test_indices], normalizeOptions)
        # predict the value, (value is normalized).
        p = np.array(list(map(reg_predict, xc))).ravel()
        e = (p+x1) - y[test_indices]
        err += np.sum(e**2)

        pMean = y[train_indices].mean()
        eMean += np.sum( (pMean - y[test_indices])**2 )

    print(err/float(eMean))




if __name__ == "__main__":
    for i in range(reviews.shape[0]):
        netflixRegPredict(regLasso, user_id = i)
        netflixRegPredict(regElast, user_id = i)
        netflixRegPredict(regLasso, user_id = i, normalizeOptions='after')
        netflixRegPredict(regElast, user_id = i, normalizeOptions='after')
