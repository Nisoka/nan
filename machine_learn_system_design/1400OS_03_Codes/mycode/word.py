#!/usr/bin/env python
# -*- coding: utf-8 -*-

'''
 3.1:
    没有标签的可以让分类模型学习, 我们不可能学习得到一个分类器, 然后可以从数据本身学习到一些模式,
    用户提出一个问题 然后返回给他最想要的答案.
    计算一个帖子和其他帖子的相似度，然后得到前N个最相似的帖子。

    通过聚类实现目的, 使相同的数据项 归类同一个簇中,
    问题1 将文本进行相似度计算?
        原始的文本用处不大,计算机又不认识, 至少现在不认识,
        转化为数学形式,  --- 词袋向量.
        #3.1.2 bag of words
        # features is words.
        # feature_value is one wordCnt in a doc.
        # strategy:
        #   1 transform doc to a vocab_Vec. (some important process)
        #   2 cluster
        #   3 confirm every doc in which cluster
        #   4

'''
import numpy as np
import scipy as sp
import os
import math
import nltk
import sklearn.datasets
from sklearn.feature_extraction.text import CountVectorizer
from sklearn.feature_extraction.text import TfidfVectorizer

MLCOMP_DIR = r"C:\Users\people\QtProject\mlapps\20news-18828"
mlcomp_data = sklearn.datasets.load_mlcomp("20news-18828", mlcomp_root=MLCOMP_DIR)




'''
3.2
    预处理,用相近的公共词语个数来衡量相似性
    3.2.1 原始文本 -> 词袋向量
    向量化处理器 : sklearn CounterVectorizer.
    3.2.2 统计词语
    将数据整体全部传入给 向量化处理器, 向量化处理器就会自动统计词语,
    并得到 词袋特征 -- vocabBag.
    通过向量化处理器 得到了文本的 词袋向量
    计算距离.

    3.2.3 归一化词频, 计算归一化距离
    对自己进行归一化 这样好么？他这样收缩了长度减小词频影响,貌似应该并不好啊.
    对于 其他非文本数据这样确实不好, 但是对于文本来说 词频影响比较小, 而更加关注于词语组合,
    即 文本匹配 更注重内在意思 - 多个词相同越多越好, 而不是一个词出现的越多越好.
    相同的词越多越好, 同一个相同的词 次数越多并不重要

    3.2.4 删除不重要的词语
    stop_words = 'english' 文本中 很多词语 并不是为了表达意思 而是为了形容的这类词语
    对文本意思并不重要 计算相似度时候 不需要.
    不重要的词 没有意义。

    3.2.5 词干处理 nltk
    词语形式不同 实际意义实际相同, 通过抽取词干, 将不同形式的词语变为同一个词语

    3.2.6 停用词兴奋剂
    有些词 对特殊的文本 具有标记型, 军事文本 一般都会出现国家名字.
    为这些特征性词语 提高权重.
    而对于某些大众词(a subject example)等 减少权重，因为任何一个文本都有很大可能出现. 甚至直接标记为停用词.
    词频-反转文档频率 (TF-IDF)
    此时词袋向量的 特征值 并不是词的统计值， 而是一个词的TF-IDF值,
    可以考虑认为是 词对于整个数据集的分类权重值.

    3.2.7 成果和目标
    文本预处理
    1 文本切分
    2 去掉stop_words 和
    3 统计剩余词语
    4 考虑整个语料集合, 从词频统计中计算每个词的 tf-idf值. 生成 以tf-idf为特征值的 词袋向量
    此时 我们将充满噪音的文本转化为了简明的 分类特征的词袋向量数据集.
    问题、缺点:
        1 词语之间关联关系
        2 not or yes 否定关系捕捉
        3 拼写错误的词语 捕捉错误.


'''
def test_counterVectorizer():
    content = ["How to format my hard disk",
                "Hard disk format problems"]
    vectorizer = CountVectorizer(min_df=1)

    X = vectorizer.fit_transform(content)
    vecNames = vectorizer.get_feature_names()

    data = X.toarray()
    print(data)

# Eul
def dist_raw(v1, v2):
    delta = v1 - v2
    return sp.linalg.norm(delta.toarray())

# norm Eul
def dist_norm(v1, v2):
    v1_norm = v1/sp.linalg.norm(v1.toarray())
    v2_norm = v2/sp.linalg.norm(v2.toarray())
    return dist_raw(v1_norm, v2_norm)

english_stemmer = nltk.stem.SnowballStemmer('english')
class StemmedCounterVectorizer(CountVectorizer):
    def build_analyzer(self):
        analyzer = super(StemmedCounterVectorizer, self).build_analyzer()
        return lambda doc: (english_stemmer.stem(w) for w in analyzer(doc))

# term 词 对 doc的重要性(权重) 在集合docset中.
def tfidf(term, doc, docset):
    tf = float(doc.count(term))/sum(doc.count(w) for w in set(doc))
    idf = float(len(docset))/(len([docTemp for docTemp in docset if term in docTemp]))
    idf = math.log(idf)
    return tf*idf

class StemmedTfidfVectorizer(TfidfVectorizer):
    def build_analyzer(self):
        analyzer = super(StemmedTfidfVectorizer, self).build_analyzer()
        return lambda doc: (english_stemmer.stem(w) for w in analyzer(doc))

def test_tfidf():
    a, abb, abc = ['a'], ['a', 'b', 'b'], ['a', 'b', 'c']
    D = [a, abb, abc]
    print(tfidf('a', a, D))
    print(tfidf('a', abb, D))
    print(tfidf('a', abc, D))
    print(tfidf('b', a, D))
    print(tfidf('b', abb, D))
    print(tfidf('b', abc, D))
    print(tfidf('c', a, D))
    print(tfidf('c', abb, D))
    print(tfidf('c', abc, D))


def loadData():
    DIR = "../data/toy/"
    posts = [open(os.path.join(DIR, f)).read() for f in os.listdir(DIR)]
    return posts

def vocabTransform(vectorizer, data_words):
    data_vocabVec = vectorizer.fit_transform(data_words)
#    print(data_vocabVec.shape)
    #get the vocabBag.
    vecNames = vectorizer.get_feature_names()
#    print(vecNames)
    return data_vocabVec

def calcNeareast(vectorizer, dataSet, dataSet_vocab, example, dist = dist_raw):
    best_doc = None
    best_dist = 999999
    best_i = None
    num_samples = len(dataSet)
    for i in range(0, num_samples):
        entry = dataSet[i]
        if entry == example:
            continue
        entry_vec = dataSet_vocab.getrow(i)
        example_vec = vectorizer.transform([example])
        d = dist(entry_vec, example_vec)
        print("============ Index: %d distance: %.2f \n%s, " %(i, d, entry))
        if d < best_dist:
            best_dist = d
            best_i = i
            best_doc = entry
    return best_i, best_dist, best_doc



def mlcomp():
    print(mlcomp_data.filenames)
    print(len(mlcomp_data.filenames))

def easyToUse():
#    vectorizer = CountVectorizer(min_df=1, stop_words = 'english')
#    vectorizer = StemmedCounterVectorizer(min_df=1, stop_words = 'english')
    vectorizer = StemmedTfidfVectorizer(min_df=1, stop_words = 'english', charest_error = 'ignore')
    data_words = loadData()
    data_vocabVec = vocabTransform(vectorizer, data_words)

    new_post = "imaging databases"
    new_post_vec = vectorizer.transform([new_post])
    print(new_post_vec.toarray())

    calcNeareast(vectorizer, data_words, data_vocabVec, new_post, dist=dist_norm)

if __name__ == "__main__":
    mlcomp()
