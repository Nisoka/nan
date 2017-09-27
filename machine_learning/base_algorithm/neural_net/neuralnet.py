#!/usr/bin/env python
# -*- coding: utf-8 -*-


import numpy as np

def nonlin(x, deriv = False):
    if(deriv == True):
        return x*(1-x)
    return 1/(1 + np.exp(-x))

def test():
    x = np.array([[0, 0, 1], [0, 1, 1], [1, 0, 1], [1, 1, 1]])
    y = np.array([ [0, 0, 1 , 1] ]).T
    np.random.seed(1)
    syn0 = 2*np.random.random( (3, 1) ) - 1

    for iter in range(10000):
        l0 = x
        l1 = nonlin(np.dot(l0, syn0))
        l1_err = y - l1
        l1_delta = l1_err * nonlin(l1, True)
        syn0 += np.dot(l0.T, l1_delta)

    print("output After Training")
    print(l1)


if __name__ == "__main__":
    test()
