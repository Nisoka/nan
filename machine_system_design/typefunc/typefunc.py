# -*- coding: utf-8 -*-

import scipy as sp
import numpy as np
import pandas as pd

# ndarry list
# list 是有逗号的, ndarray 没有逗号
# list 不能进行高维切片,
# 因为 list 不能用[a, b] tuple访问元素, 只能维度索引[a][b]
#    list[:] 得到的 还是原本的list，没有像ndarray一样取出来.
#    list 只能访问每个元素,  而不能多维切片, 只能实现最高维一个维度的切片
#    而 ndarray 就可以通过 [a, b] 可以进行多维度切片.

#list 不能用list索引

def ndarray2float(ndarray):
    return np.array(ndarray, dtype = 'float32')

def list2ndarray(list):
    return np.array(list, dtype='float32')

def ndarray2list(ndarray):
    return ndarray.tolist()
