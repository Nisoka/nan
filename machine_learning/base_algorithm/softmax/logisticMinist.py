# -*- coding: utf-8 -*-
"""
Created on Tue May 16 21:25:15 2017

@author: nan
"""

import time
import math
import random
import pandas as pd
import load_mnist
import numpy as np

from sklearn.cross_validation import train_test_split
from sklearn.metrics import accuracy_score

class LogisticRegression():
    
    def __init__(self):
        self.learning_setp = 0.00001
        self.max_iteration = 5000
        
    def predict_(self, x):
        wx = sum([self.w[j]*x[j] for j in range(len(self.w))])
        exp_wx = math.exp(wx)
        predict = exp_wx/(1 + exp_wx)
        
        if predict > 0.5:
            return 1
        else:
            return 0
    
    def train(self, features, labels):
        self.w = [0.0] *(len(features[0]) + 1)
        correct_count = 0
        time = 0
        
        while time < self.max_iteration:
            index = random.randint(0, len(labels)-1)
            x = list(features[index])
            x.append(1.0)   #bias
            y = labels[index]
            
            if y == self.predict_(x):
                correct_count += 1
                if correct_count > self.max_iteration:
                    break
                continue
            
            time += 1
            correct_count = 0
            # x*w => wx
            wx = sum([self.w[i] * x[i] for i in range(len(self.w))])
            exp_wx = math.exp(wx)
            for i in range(len(self.w)):
                self.w[i] -= self.learning_setp*(-y*x[i] + \
                      float(x[i]*exp_wx)/float(1+exp_wx))
    
    def predict(self, features):
        labels = [] 
        for feature in features:
            x = list(feature)
            x.append(1.0)
            labels.append(self.predict_(x))
        
        return labels


if __name__ == "__main__":
    print('Start read data')
    time_1 = time.time()
    data_x, data_y = load_mnist.loadDataSet("../datas/digits/trainingDigits")

    print("Starting training !")
    lr = LogisticRegression()
    lr.train(data_x, data_y)
    
    data_x_t, data_y_t = load_mnist.loadDataSet("../datas/digits/testDigits")
    test_predict = lr.predict(data_x_t)
    
    score = accuracy_score(data_y_t, test_predict)
    print("The accuracy score is ", score)
