#!/usr/bin/env python
# -*- coding: utf-8 -*-

import numpy as np
import sys
sys.path.insert(0, "../")
from bookCode import load_ml100k as ml100k
from matplotlib import pyplot as plt
from sklearn.cross_validation import KFold

def loadData():
    reviews = ml100k.load('../data/ml-100k/u.data')
    return reviews

def imageShow(reviews):
    imagedata = reviews[:200, :200].todense()
    print(imagedata)
    plt.imshow(imagedata, interpolation='nearest')
    plt.show()


def all_correlations(bait, target):
    '''
    corrs = all_correlations(bait, target)
    corrs[i] is the correlation between bait and target[i]
    '''
    return np.array( [np.corrcoef(bait, c)[0, 1] for c in target])


###############################################
# base on user similar
###############################################

def estimate(user, rest):
    '''
    estimate movie ratings for 'user' based on the 'rest' of the universe
    '''
    bu = user > 0
    br = rest > 0
    relations = all_correlations(bu, br)
    selected_indices = relations.argsort()[-100:]
    estimate_value = rest[selected_indices].mean(axis = 0)  # axis = 0|
    # some movie have more user .
    estimate_value /= (0.1 + br[selected_indices].mean(axis = 0)) # axis = 0 |
    return estimate_value

def estimate_error(user, rest):
    #estimate user values.
    estimate_value = estimate(user, rest)
    bu = user > 0
    br = rest > 0
    # estimate_value - user_value.
    error = estimate_value[bu] - user[bu]

    rest_mean = rest.mean(axis = 0)  # |
    rest_mean /= (0.1 + br.mean(axis = 0))
    eMean = rest_mean[bu] - user[bu]
    return np.sum(error**2), np.sum(eMean**2)

def netflix_userSimiler():
    reviews = loadData().toarray()
    errList = []
    # all user estimate.
    for i in range(reviews.shape[0]):
        errList.append(estimate_error(reviews[i], np.delete(reviews, i, 0)))

    error = np.array(errList)
    err = error.sum(axis = 0)
    print(err)



###############################################
# base on movie similar
###############################################

def movie_like(reviews):
    nmovies = reviews.shape[1]
    print("calc the movie_like in", nmovies)
    movie_likeness = np.zeros( (nmovies, nmovies) )
    allms = np.ones(nmovies, bool)
    for i in range(nmovies):
        movie_likeness[i] = all_correlations(reviews[:, i], reviews.T)
        movie_likeness[i, i] = -1

    print("calc the movie_like out")
    return movie_likeness

def estimate_movie(movie_likeness, reviews, uid, mid):
    likes = movie_likeness[mid].argsort()
    # 逆序
    likes = likes[::-1]

    for ell in likes:
        if reviews[uid,ell] > 0:
            return reviews[uid, ell]

def estimate_error_movie(movie_likeness, reviews, uid):
    user_movies = reviews[uid]
    rest_movies = np.delete(reviews, uid, 0)
    m_indices = user_movies > 0

    estimate_value = [estimate_movie(movie_likeness, reviews, uid, mid) for mid in m_indices]
    error = estimate_value[m_indices] - user_movies[m_indices]

#    rest_mean = rest.mean(axis = 0)  # |
#    rest_mean /= (0.1 + br.mean(axis = 0))
#    eMean = rest_mean[bu] - user[bu]
    return error

def netflix_movieSimiler():
    reviews = loadData().toarray()

    movie_likeness = movie_like(reviews)
    print(movie_likeness)

    errList = []
    for userid in range(len(reviews)):
        errList.append(estimate_error_movie(movie_likeness, reviews, userid))
    error = np.array(errList)
    rmse = np.sqrt(error.sum(axis = 0))
    print(rmse)

if __name__ == "__main__":
#    netflix_userSimiler()
    netflix_movieSimiler()
