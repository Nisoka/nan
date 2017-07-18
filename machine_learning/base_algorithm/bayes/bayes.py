#!/usr/bin/env python
# -*- coding: utf-8 -*-

from numpy import *

def loadDataSet():
    postingList=[['my', 'dog', 'has', 'flea', 'problems', 'help', 'please'],
                 ['maybe', 'not', 'take', 'him', 'to', 'dog', 'park', 'stupid'],
                 ['my', 'dalmation', 'is', 'so', 'cute', 'I', 'love', 'him'],
                 ['stop', 'posting', 'stupid', 'worthless', 'garbage'],
                 ['mr', 'licks', 'ate', 'my', 'steak', 'how', 'to', 'stop', 'him'],
                 ['quit', 'buying', 'worthless', 'dog', 'food', 'stupid']]
    classVec = [0,1,0,1,0,1]    #1 is abusive, 0 not
    return postingList,classVec

'''
    创建 dataSet 的单词集合列表
    输入:
        dataSet     输入训练数据集
            list[list_doc,list]
            list_doc    是每个文件中所有单词文字.
    内容:
        1 vocabSet    单词集合
        2 document in dataSet dataSet 中每个文件遍历
        3 set(docment) 对文件构建一个 set集合 保存包含的单词，并且每个单词不可重复
        4 vocabSet | set(document)  组合集合
        5 返回一个 vocabSet 构成的list
    输出:
'''
def createVocabList(dataSet):
    vocabSet = set([])          #create empty set
    for document in dataSet:    #iterater all the dataSet to create the words vocabList.
        vocabSet = vocabSet | set(document)     # use set or attribute.
    return list(vocabSet)

'''
    将inputSet 映射到 vocabList 代表的 单词 特征空间.
    存在的单词 +=1 不存在的表示为0
    结果每个inputSet 都变成了vocabList特征空间中的向量.
    相当于 用vocabList 可以描述了inputSet。
'''
def words2Vec(vocabList, inputSet):
    # 创建一个全 0 向量
    returnVec = [0]*len(vocabList)
    for word in inputSet:
        if word in vocabList:
            returnVec[vocabList.index(word)] += 1
        else:
            print("the word -%s- is not in the vocabList!" % word)

    return returnVec

'''
# NB Navie Bayes 概率向量
    trainMatrix --- 每个条目entry 生成的特征向量(词袋向量)构成的训练X
        mat 每个条目entry 都是 doc 生成的vocabList，所以长度都已经一样了.
            且变为了 int_vec的类型。
    trainLabels --- 每个条目标签 0 normal 1 abusive(辱骂)
'''
def trainNBVecP(trainMatrix, trainLabels):
    numTrainDocs = len(trainMatrix)
    numWords = len(trainMatrix[0])
    pAbusive = sum(trainLabels)/float(numTrainDocs)

    # p0Num 正常类 词向量统计
    # p1Num 谩骂类 词向量统计
    # p0Denom   正常类 词总数统计
    # p0Denom   谩骂类 词总数统计
    p0Num = ones(numWords)
    p1Num = ones(numWords)
    p0Denom = 2.0
    p1Denom = 2.0
    for i in range(numTrainDocs):
        if trainLabels[i] == 1:
            p1Num += trainMatrix[i]
            p1Denom += sum(trainMatrix[i])
        else:
            p0Num += trainMatrix[i]
            p0Denom += sum(trainMatrix[i])
    # 正常类  词概率向量取 log
    # 谩骂类  词概率向量取 log
    p0Vec = log(p0Num/p0Denom)
    p1Vec = log(p1Num/p1Denom)

    return p0Vec, p1Vec, pAbusive

'''
    Navie Bayes 朴素贝叶斯 分类函数
    输入:
        vec2Classify    待分类词向量
        p0Vec, p1Vec    正常类、谩骂类词概率向量
        pClass1         谩骂类概率
    内容:
        1 贝叶斯公式 -- 后验概率公式  P(A|B) = P(B|A)*P(A)/P(B)
        2 后验概率 直接判断
'''
def classifyNB(vec2Classify, p0Vec, p1Vec, pClass1):
    # 正常类 词向量 经过正常词向量概率乘 得到 该词向量是 正常类的概率
    # 比较 两个概率, 由此判别
    p0 = sum(vec2Classify*p0Vec) + log(1.0 - pClass1)
    p1 = sum(vec2Classify*p1Vec) + log(pClass1)

    if p1 > p0:
        return 1
    else:
        return 0



#def testingNB():
#    listOPosts, listClasses = loadDataSet()

#    myVocabList = createVocabList(listOPosts)

#    trainMat = []
#    for postingDoc in listOPosts:
#        trainMat.append(words2Vec(myVocabList, postingDoc))

#    p0V, p1V, pAb = trainNBVecP(array(trainMat), array(listClasses))

#    testEntry = ['love', 'my', 'dalmation']
#    thisDoc = array(words2Vec(myVocabList, testEntry))
#    print(testEntry, "classify is ", classifyNB(thisDoc, p0V, p1V, pAb))


#    testEntry = ['stupid', 'garbage']
#    thisDoc = array(words2Vec(myVocabList, testEntry))
#    print(testEntry, "classify is ", classifyNB(thisDoc, p0V, p1V, pAb))


class cNavieBayes:
    def __init__(self, data_x, data_y):
        self.data_x = data_x
        self.data_y = data_y
        self.m = shape(data_x)[0]
        self.p0Vec = []
        self.p1Vec = []
        self.p1    = 0.0

    def train(self):
        self.p0Vec, self.p1Vec, self.p1 = trainNBVecP(self.data_x, self.data_y)
        return self.p0Vec, self.p1Vec, self.p1

    def predict(self, entry_x):
        ret = classifyNB(entry_x, self.p0Vec, self.p1Vec, self.p1)
        return ret

    def trainErr(self):
        error = 0.0
        for i in range(self.m):
            entry_x = self.data_x[i,:]
            # print(entry_x)
            ret = self.predict(entry_x)
            # print(int(ret), int(self.data_y[i]))
            if int(ret) != int(self.data_y[i]):
                error += 1
        # print(self.data_y)
        error = float(error)/float(self.m)
        return error



def testingNB():
    listOPosts, listClasses = loadDataSet()

    myVocabList = createVocabList(listOPosts)

    trainMat = []
    for postingDoc in listOPosts:
        trainMat.append(words2Vec(myVocabList, postingDoc))

    navieBayes = cNavieBayes(array(trainMat), array(listClasses))
    p0V, p1V, pAb = navieBayes.train()
    print(navieBayes.trainErr())
    testEntry = ['love', 'my', 'dalmation']
    thisDoc = array(words2Vec(myVocabList, testEntry))
    print(testEntry, "classify is ", navieBayes.predict(thisDoc))


    testEntry = ['stupid', 'garbage']
    thisDoc = array(words2Vec(myVocabList, testEntry))
    print(testEntry, "classify is ", navieBayes.predict(thisDoc))

'''
    emil test
'''


'''
    格式化 文本为 词语列表
'''
def textParse(bigString):
    import re
    listOfTokens = re.split(r'\W*', bigString)
    return [tok.lower() for tok in listOfTokens if len(tok) > 2]

'''
    spam 垃圾邮件
    内容:
        docList     训练数据列表
        classList   训练数据标签列表
        fullText    全部单词???
        1 先读取文件，格式化为词语列表，添加到docList构建训练数据列表 和 标签列表.
        2 利用 docList训练数据列表 构建 词向量
        3 抽取 0 - 50 中的 10 index索引 作为 testSet
        4 构建词向量数据集 trainMat classLabels
        5 构造类 进行训练
        6 利用testTest索引进行测试
'''
def spamTest():
    # docList
    docList = []
    classList = []
    fullText = []
    for i in range(1, 26):
        # print(i)
        wordList = textParse(open('email/spam/%d.txt' % i).read())
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(1)
        wordList = textParse(open('email/ham/%d.txt' % i).read())
        docList.append(wordList)
        fullText.extend(wordList)
        classList.append(0)

    vocabList = createVocabList(docList)
    trainSet = list(range(50))
    testSet = []
    for i in range(10):
        randIndex = int(random.uniform(0, len(trainSet)))
        testSet.append(trainSet[randIndex])
        del(trainSet[randIndex])

    trainMat = []
    classLabels = []
    for docIndex in trainSet:
        trainMat.append(words2Vec(vocabList, docList[docIndex]))
        classLabels.append(classList[docIndex])

    print("Begin to Train!!")
    navieBayes = cNavieBayes(array(trainMat), array(classLabels))
    p0V, p1V, pSpam = navieBayes.train()
    print(navieBayes.trainErr())

    errorCnt = 0
    for docIndex in testSet:
        wordsVec = words2Vec(vocabList, docList[docIndex])
        if navieBayes.predict(array(wordsVec)) != classList[docIndex]:
            errorCnt += 1
            print("Error ", docList[docIndex])

    print("Error cnt is %d, sum is %d" % (errorCnt, len(testSet)))


def easyToUse():
    # testingNB()
    spamTest()

if __name__ == "__main__":
    easyToUse()
