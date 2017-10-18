#!/usr/bin/env python
# -*- coding: utf-8 -*-


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

def train_bayes_nb(data_x, data_y):


