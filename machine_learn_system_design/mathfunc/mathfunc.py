# -*- coding: utf-8 -*-

import scipy as sp
import numpy as np

def distance_euc(vec0, vec1):
    'Computes squred euclidean distance'
    return np.sum( (vec0 - vec1)**2 )

