

import numpy as np
import matplotlib.pyplot as plt


def load_data(filename):
    data_x = []
    data_y = []
    fr = open(filename)
    for line in fr.readlines():
        features = line.strip().split('\t')
        data_x.append([1.0, float(features[0]), float(features[1])])
        data_y.append(int(features[2]))
    return np.array(data_x), np.array(data_y)


def sigmoid(x):
    return np.longfloat(1.0/(1 + np.exp(-x)))

def grad_ascent(data_x, data_y, max_iter = 500):
    m, n = np.shape(data_x)
    alpha = 0.001
    Weights = np.ones((n, 1))
    
    for i in range(max_iter):
        temp_y = np.dot(data_x, Weights)
        y_hat = sigmoid(temp_y)
        print(y_hat.shape, data_y.shape)
        #(m, 1)
        error = data_y - y_hat    
        #(n, 1) += alpha * (n, m) * (m, 1)
        print(data_x.shape, error.shape)
        Weights += alpha*np.dot(data_x.transpose(), error)

    return Weights

def classify(entry, Weights):
    y = sigmoid(np.dot(entry, Weights))
    if y > 0.5:
        return 1
    else:
        return 0

class cLogisticReg:
    def __init__(self, data_x, data_y, maxIter):
        self.data_x = np.array(data_x)
        self.m = self.data_x.shape[0]
        self.n = self.data_x.shape[1]
        self.data_y = np.array(data_y).reshape((self.m, 1))
        self.Weights = np.ones((self.n, 1))
        self.max_iter = maxIter
        
    def train(self):
        self.Weights = grad_ascent(self.data_x, self.data_y, self.max_iter)
        return self.Weights

    def predict(self, entry):
        ret = classify(entry, self.Weights)
        return ret
    
    def train_err(self):
        err_cnt = 0
        for i in range(self.m):
            entry_vec = self.data_x[i]
            entry_vec = entry_vec.reshape((1, self.n))
            ret = self.predict(entry_vec)
            if ret != self.data_y[i]:
                err_cnt += 1
        print("data cnt is %d, and err cnt is %d" %(self.m, err_cnt))
        return float(err_cnt)/self.m
        

def str2float(str):
    return float(str)

def horseColicTest():
    file = open('horseColicTraining.txt')
    data_x = []
    data_y = []
    
    for line in file.readlines():
        features = line.strip().split('\t')
        data_x.append(list(map(str2float, features[:-1])))
        data_y.append(str2float(features[-1]))
        
    
    logistic = cLogisticReg(data_x, data_y, 800)
    Weights = logistic.train()
    #print(Weights)
    logistic.train_err()

    file_test = open('horseColicTest.txt')
    err_cnt = 0
    test_cnt = 0
    for line in file_test.readlines():
        test_cnt += 1
        entry = line.strip().split('\t')
        entry_vec = np.array(list(map(str2float, entry[:-1])))
        entry_vec.reshape( (1, len(entry_vec)) )
        ret = logistic.predict(entry_vec)
        if ret != int(entry[-1]):
            err_cnt += 1
    
    print("data cnt is %d, err cnt is %d " %(test_cnt, err_cnt))


def dataSetTest():
    dataMat, labelMat = load_data('testSet.txt')
    m = len(labelMat)
    labelMat = np.mat(labelMat)
    labelMat = labelMat.reshape( (m, 1) )
    Weights = grad_ascent(np.mat(dataMat), labelMat)
    print(Weights)
    errCnt = 0
    for i in range(len(dataMat)):
        result = classify(dataMat[i], Weights)
        if result != labelMat[i]:
            errCnt += 1
    print(errCnt, m)

        
if __name__ == '__main__':
#    dataSetTest()
    horseColicTest()
