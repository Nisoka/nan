#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import math

def softmax(x):
    x = x - np.max(x)
    exp_x = np.exp(x)
    #print(exp_x)
    softmax_x = exp_x / np.sum(exp_x)
    return softmax_x

if __name__ == "__main__":
    x = [1000, 2000, 3000]
    print(softmax(x))
