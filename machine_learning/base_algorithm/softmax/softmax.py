#!/usr/bin/env python
# -*- coding: utf-8 -*-

import load_mnist
import numpy as np


#dataSet, labels = load_mnist.loadDataSet("trainingDigits")
#print(labels)

def softMax(y_vec):
    #(1, k) = (1, n) * (n, k)
    exp_y = np.exp(y_vec)
    pro_vec = exp_y/np.sum(exp_y, axis = 0)
    #print("softmax : ", pro_vec)
    return pro_vec

#def softMax(vec):
#    #vec = x*w = (1, k)
#    vec = vec - np.max(vec, axis = 0)
#    # (1, k)
#    expVec = np.exp(vec)
#    softMaxVec = expVec/np.sum(expVec, axis = 0)
#    return softMaxVec

def one_hot(labels, labelsCnt):
    oneHotMat = np.zeros((len(labels), labelsCnt))
    for i in range(len(labels)):
        oneHotMat[i, labels[i]] = 1.0
    return oneHotMat

class cSoftMax():
    def __init__(self, dataMat, labels):
        self.m = dataMat.shape[0]
        self.n = dataMat.shape[1]
        self.dataMat = np.concatenate([np.ones((dataMat.shape[0], 1)), dataMat], axis=1)
        self.labelMat = one_hot(labels, 10)
        self.alpha = 0.00001
        self.theta = np.ones( (10, self.n+1) )

    def probability(self, entry):
        lineVec = np.dot(self.theta, entry.T)
        proVec = softMax(lineVec)
        return proVec

    def train(self):
        # gradientAscent
        # deltaj = sumXi{ -(xi (1{yi == j} - p(yi = j|xi,theta)) }
        # thetaJ = [thetaJ0, thetaJ1, thetaJ2, thetaJn+1]
        # 对于 yi == j thetaj += alpha( -xi (1-p))
        # 对于 yi != j thetaj += alpha( xi * p)
        # deltaji = -self.dataMat * (self.labelMat.T - probability(self.dataMat))
        #           - mx(n+1) * (10*m - 10*m)
        # deltaji = -(self.labelMat.T - probability(self.dataMat)) * self.dataMat
        #           - (10xm - 10xm) * mx(n+1)
        #           - 10x(n+1){maybe is 0} + 10x(n+1）

        iter = 1
        while(iter < 1000):
            proVec = self.probability(self.dataMat)
            deltaMat = -(self.labelMat.T - proVec)* self.dataMat
#            print(self.labelMat.T)
#            print(proVec)
#            print(deltaMat)
#            print(deltaMat.shape)
            self.theta -= self.alpha*deltaMat
            print(self.theta)
            iter += 1

#        print(self.theta)

    def test(self, testDataMat, labels):
        testDataMat = np.concatenate([np.ones((testDataMat.shape[0], 1)), testDataMat], axis=1)
        lineVec = np.dot(self.theta, testDataMat.T)
        prolist = np.argmax(softMax(lineVec), axis = 0)[0].tolist()[0]
#        print(prolist)
        errCnt = 0
        testLen = len(prolist)
        for i in range(testLen):
            if prolist[i] != labels[i]:
                errCnt += 1
        print("Error Cnt is %d, testCnt is %d, the recognize rate is %f" % (errCnt, testLen, 1-float(errCnt)/testLen))



def main():
    dataSet, labels = load_mnist.loadDataSet("../datas/digits/trainingDigits")
    sf = cSoftMax(np.mat(dataSet), labels)
    sf.train()
    dataSet, labels = load_mnist.loadDataSet("../datas/digits/testDigits")
    sf.test(np.mat(dataSet), labels)


if __name__ == "__main__":
    main()

