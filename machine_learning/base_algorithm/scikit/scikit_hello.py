#!/usr/bin/env python
# -*- coding: utf-8 -*-

from sklearn import datasets
iris = datasets.load_iris()
digits = datasets.load_digits()

print(digits.data)
