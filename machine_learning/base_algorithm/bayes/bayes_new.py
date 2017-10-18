#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *

def load_testdata():
    data_x =[['my', 'dog', 'has', 'flea', 'problems', 'help', 'please'],
            ['maybe', 'not', 'take', 'him', 'to', 'dog', 'park', 'stupid'],
            ['my', 'dalmation', 'is', 'so', 'cute', 'I', 'love', 'him'],
            ['stop', 'posting', 'stupid', 'worthless', 'garbage'],
            ['mr', 'licks', 'ate', 'my', 'steak', 'how', 'to', 'stop', 'him'],
            ['quit', 'buying', 'worthless', 'dog', 'food', 'stupid']]
    data_y = [0, 1, 0, 1, 0, 1]
    return data_x, data_y

def create_vocablist(data_x):
    vocab_set = set([])
    for entry in data_x:
        vocab_set = vocab_set | set(entry)
    return list(vocab_set)

def words2Vec(entry, vocablist):
    entry_vec = [0]*len(vocablist)
    for word in entry:
        if word in vocablist:
            entry_vec[vocablist.index(word)] += 1
        else:
            print("word is not in the vocablist!")
    return entry_vec

def train_navie_bayes(data_x, data_y):
    data_cnt = len(data_x)
    data_n = len(data_x[0])
    p1 = sum(data_y)/float(data_cnt)
    
    p0_vec = ones(data_n)
    p1_vec = ones(data_n)
    p0_cnt = 2.0
    p1_cnt = 2.0

    for i in range(data_cnt):
        if data_y[i] == 0:
            p0_vec += data_x[i]
            p0_cnt += sum(data_x[i])
        else:
            p1_vec += data_x[i]
            p1_cnt += sum(data_x[i])

    p0_vec = log(p0_vec/p0_cnt)
    p1_vec = log(p1_vec/p1_cnt)
    
    return p0_vec, p1_vec, p1


def classify(entry_vec, p0_vec, p1_vec, p1):
    res_p0 = sum(entry_vec*p0_vec) + log(1.0 - p1)
    res_p1 = sum(entry_vec*p1_vec) + log(p1)
    
    if res_p1 > res_p0:
        return 1
    else:
        return 0


class cNavieBayes:
    def __init__(self, data_x, data_y):
        self.data_x = data_x
        self.data_y = data_y
        self.m = shape(data_x)[0]
        self.n = shape(data_x)[1]
        self.p0_vec = []
        self.p1_vec = []
        self.p1 = 0.0
        
    def train(self):
        self.p0_vec, self.p1_vec, self.p1 = train_navie_bayes(self.data_x, 
                                                              self.data_y)
        return self.p0_vec, self.p1_vec, self.p1

    def predict(self, entry_vec):
        ret = classify(entry_vec, self.p0_vec, self.p1_vec, self.p1)
        return ret

    def train_error(self):
        err_cnt = 0
        for i in range(self.m):
            entry_vec = self.data_x[i]
            ret = self.predict(entry_vec)
            if ret != self.data_y[i]:
                err_cnt += 1
        error = float(err_cnt)/self.m
        return error

def testingNB():
    data_x_origin, data_y = load_testdata()
    vocablist = create_vocablist(data_x_origin)
    print(vocablist)

    m = len(data_x_origin)
    data_x = []
    for i in range(m):
        entry_vec = words2Vec(data_x_origin[i], vocablist)
        data_x.append(entry_vec)
        
    bayes_obj = cNavieBayes(data_x, data_y)
    bayes_obj.train()
    print(bayes_obj.train_error())
    
    test_entry = ['love', 'my', 'dalmation']
    entry_vec = words2Vec(test_entry, vocablist)
    print(test_entry, " class is ", bayes_obj.predict(entry_vec))

    test_entry = ['stupid', 'garbage']
    entry_vec = words2Vec(test_entry, vocablist)
    print(test_entry, " class is ", bayes_obj.predict(entry_vec))        
        

def text_parse(long_string):
    import re
    tmp_list = re.split(r'\W*', long_string)
    return [tok.lower() for tok in tmp_list if len(tmp_list) > 2]

def spamTest():
    doc_list = []
    label_list = []
    train_end = 0.7

    for i in range(26):
        word_list = text_parse(open("email/spam/%d.txt" %i).read())
        doc_list.append(word_list)
        label_list.append(1)
        word_list = text_parse(open("email/ham/%d.txt" %i).read())
        doc_list.append(word_list)
        label_list.append(0)

    vocablist = create_vocablist(doc_list)
    
    data_cnt = len(doc_list)
    train_end = int(train_end*data_cnt)
    data_x = []
    data_y = []
    for i in range(train_end):
        rand_index = int(random.uniform(0, len(doc_list)))
        entry_vec = words2Vec(doc_list[rand_index], vocablist)
        data_x.append(entry_vec)
        data_y.append(label_list[rand_index])
        del(doc_list[rand_index])
        del(label_list[rand_index])

    
    navieBayes = cNavieBayes(data_x, data_y)
    navieBayes.train()
    
    data_cnt_t = len(doc_list)
    err_cnt = 0
    for i in range(data_cnt_t):
        entry_vec = words2Vec(doc_list[i], vocablist)
        ret = navieBayes.predict(entry_vec)
        if ret != label_list[i]:
            err_cnt += 1
            
    error = float(err_cnt)/data_cnt_t
    print("Cnt is %d, Error Cnt is %d, and the error ratio is %f" %(data_cnt_t, err_cnt, error))
        
    


if __name__ == '__main__':
    spamTest()



    
