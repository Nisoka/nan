#!/usr/bin/env python
# -*- coding: utf-8 -*-

import load_mnist
import numpy as np

def softmax(inx, Weights):
    #(1, k) = (1, n) * (n, k)
    y_vec = np.dot(inx, Weights)
    exp_y = np.exp(y_vec)
    pro_vec = exp_y/np.sum(exp_y, axis = 0)
    #print("softmax : ", pro_vec)
    return pro_vec

#def softmax(inx, Weights):
#    #vec = x*w = (1, k)
#    vec = np.dot(inx, Weights)
#    vec = vec - np.max(vec, axis = 0)
#    # (1, k)
#    expVec = np.exp(vec)
#    softMaxVec = expVec/np.sum(expVec, axis = 0)
#    return softMaxVec

def grad_ascent(data_x, data_y, Weights):
    max_iter = 100
    alpha = 0.001
    for i in range(max_iter):
        pro_vec = softmax(data_x, Weights)
        error = data_y - pro_vec
        error = -np.dot(data_x.transpose(), error)
        Weights -= alpha*error

    return Weights


def grad_ascent2(data_x, data_y, Weights):
    iter = 1
    alpha = 0.0001
    while(iter < 1000):
        pro_vec = softmax(data_x, Weights)
        error = data_y - pro_vec
        error_delta = -np.dot(data_x.transpose(), error)
        Weights -= alpha*error_delta
        iter += 1

def one_hot(labels, labelsCnt):
    oneHotMat = np.zeros((len(labels), labelsCnt))
    for i in range(len(labels)):
        oneHotMat[i, labels[i]] = 1.0
    return oneHotMat

def get_prov_class(pro_v):
    return np.argmax(pro_v, axis = 0)

class cSoftmax:
    def __init__(self, data_x, data_y, class_cnt):
        self.data_x = np.array(data_x)
        self.m = self.data_x.shape[0]
        self.n = self.data_x.shape[1]
        self.data_y = np.array(data_y)  #(m, k)
        self.alpha = 0.001
        self.class_cnt = class_cnt
        #Weights (n, k)  y_v = x*w = (m, k) argmax(y_v) is class
        self.Weights = np.ones( (self.n, self.class_cnt) )

    def train(self):
        grad_ascent2(self.data_x, self.data_y, self.Weights)

    def predict(self, entry_vec):
        pro_v = softmax(entry_vec, self.Weights)
        # 1 = argmax(1, k)
        ck = get_prov_class(pro_v)
        return ck

def test_mnist():
    data_x, data_y = load_mnist.loadDataSet("../datas/digits/trainingDigits")
    data_x = np.array(data_x)
    print(data_x.shape)
    data_x = np.concatenate([np.ones((data_x.shape[0], 1)), data_x], axis=1)
    data_y = one_hot(data_y, 10)
    sf = cSoftmax(data_x, data_y, 10)
    sf.train()

    data_x_t, data_y_t = load_mnist.loadDataSet("../datas/digits/testDigits")
    data_x_t = np.array(data_x_t)
    data_x_t = np.concatenate([np.ones((data_x_t.shape[0], 1)), data_x_t], axis=1)
    data_y_t = one_hot(data_y_t, 10)
    err_cnt = 0
    for i in range(len(data_y_t)):
        y_hat = sf.predict(data_x_t[i])
        y = np.argmax(data_y_t[i])
        print(y, y_hat)
        if y != y_hat:
            err_cnt += 1
    print(err_cnt, len(data_y_t), float(err_cnt)/len(data_y_t))
if __name__ == '__main__':
    test_mnist()
