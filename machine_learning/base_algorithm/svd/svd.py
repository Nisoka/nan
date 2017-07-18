# -*- coding: utf-8 -*-
__author__ = 'liu.junnan'

from numpy import *
import numpy.linalg as la
import matplotlib.pyplot as plt
from matplotlib.patches import *
import string

'''
    nonzero(ndArry condition);
    ndarray condition  返回的是 ndarray 通形状的 True false ndarray
    nonzero() 会返回一个 描述性的列表，高维度 -- 低维度 的一个定位描述
        eg (array([0, 3], dtype=int32), array([0, 0], dtype=int32))
'''
def loadExData():
    return[[0, 0, 0, 2, 2],
           [0, 0, 0, 3, 3],
           [0, 0, 0, 1, 1],
           [1, 1, 1, 0, 0],
           [2, 2, 2, 0, 0],
           [5, 5, 5, 0, 0],
           [1, 1, 1, 0, 0]]

def ecludSim(inA, inB):
    return 1.0/(1.0 + la.norm(inA - inB))

def pearsSim(inA, inB):
    if len(inA) < 3:
        return 1.0
    return 0.5 + 0.5*corrcoef(inA, inB, rowvar = 0)[0][1]

def cosSim(inA, inB):
    num = float(inA.T*inB)
    denom = la.norm(inA)*la.norm(inB)
    return 0.5+0.5*(num/denom)


def standEst(dataMat, user, simMeas, item):
    n = shape(dataMat)[1]
    simTotal = 0.0
    ratSimTotal = 0.0
    for j in range(n):
        userRating = dataMat[user, j]
        if userRating == 0:
            continue
        # logical_and(dataMat[:, item].A > 0, dataMat[:, j].A > 0) 返回同时满足条件的 情况矩阵True false
        # nonezero( logical condition) 返回描述 True 情况的矩阵
        # nonzero[0] 获得 condition条件下情况矩阵的 高维度
        # 此时 就获得了多个 Users [B, C, D] , Users 同时给分了 item(UserA 未评分的目标) 和 j (UserA 进行评分了的物品)
        # 这样 UserA 对item 的评分 就可以根据 这些 Users 对同时对 Item 和 j的评分关系 做一个关系系数。
        # 通过使用 相关关系公式 simMeas 计算出 Item 和 J的相关关系 similarity, 以及用户UserA 对 J 的评分UserArateJ.
        # 那么 similarity * UserArateJ 就是 预测了 UserA 根据已评价的J 可能会对Item的评分
        # 所有UserA 进行过评分的 J ，都可可能对Item 的预测 有关系，那么都累积起来，但是还要做一个平均。  ratSimTotal/simTotal.即可。
        overLap = nonzero(logical_and(dataMat[:, item].A > 0, \
                                      dataMat[:, j].A > 0))[0]

        #print(">>> ", j, overLap)
        if len(overLap) == 0:
            similarity = 0
        else:
            #print(dataMat[overLap, item])
            #print(dataMat[overLap, j])
            similarity = simMeas(dataMat[overLap, item], \
                                 dataMat[overLap, j])

        print('the Item %d and %d similarity is %f' %(item, j, similarity))
        simTotal += similarity
        ratSimTotal += similarity * userRating
    if simTotal == 0:
        return 0
    else:
        return ratSimTotal/simTotal

def svdEst(dataMat, user, simMeas, item):
    n = shape(dataMat)[1]
    simTotal = 0.0
    ratSimTotal = 0.0
    U,Sigma,VT = la.svd(dataMat)
    Sig4 = mat(eye(4)*Sigma[:4]) #arrange Sig4 into a diagonal matrix
    xformedItems = dataMat.T * U[:,:4] * Sig4.I  #create transformed items

    for j in range(n):
        userRating = dataMat[user,j]
        if userRating == 0 or j==item:
            continue
        similarity = simMeas(xformedItems[item,:].T,\
                             xformedItems[j,:].T)
        print('the Item %d and j %d similarity is: %f' % (item, j, similarity))
        simTotal += similarity
        ratSimTotal += similarity * userRating

    if simTotal == 0:
        return 0
    else:
        return ratSimTotal/simTotal

def recommend(dataMat, user, N=3, simMeas = cosSim, estMethod=standEst):
    unrateItems = nonzero(dataMat[user, :].A == 0)[1]
    if len(unrateItems) == 0:
        return 'you rated everything'
    itemScores = []
    for item in unrateItems:
        estimatedScore = estMethod(dataMat, user, simMeas, item)
        itemScores.append((item, estimatedScore))
    return sorted(itemScores, key=lambda jj: jj[1], reverse = True)[:N]
        

def printMat(inMat, thresh=0.8):
    for i in range(32):
        tempList = []
        for k in range(32):
            if float(inMat[i,k]) > thresh:
                tempList.append('*')
            else:
                tempList.append(' ')
        print(''.join(tempList))

def imgCompress(numSV=3, thresh=0.8):
    myl = []
    for line in open('0_5.txt').readlines():
        newRow = []
        for i in range(32):
            newRow.append(int(line[i]))
        myl.append(newRow)
    myMat = mat(myl)
    print("****original matrix******")
    printMat(myMat, thresh)
    U,Sigma,VT = la.svd(myMat)
    SigRecon = mat(zeros((numSV, numSV)))
    for k in range(numSV):#construct diagonal matrix from vector
        SigRecon[k,k] = Sigma[k]
    reconMat = U[:,:numSV]*SigRecon*VT[:numSV,:]
    print(U[:,:numSV])
    print(VT[:numSV,:])
    print("****reconstructed matrix using %d singular values******" % numSV)
    printMat(reconMat, thresh)


def test():
    Data = loadExData()
    myMat = mat(Data)
    U, sigma, VT = linalg.svd(Data)
    matSig = mat([[sigma[0], 0, 0], [0, sigma[1], 0], [0, 0, sigma[2]]])
    ret = U[:, :3]*matSig*VT[:3, :]
    
    print(ecludSim(myMat[:, 0], myMat[:, 4]))
    print(pearsSim(myMat[:, 0], myMat[:, 4]))
    print(cosSim(myMat[:, 0], myMat[:, 4]))

def easyToUse():
#    myMat = mat(loadExData())
#    myMat[0, 1] = myMat[0, 0] = myMat[1, 0] = myMat[2, 0] = 4
#    myMat[3, 3] = 2
#    print(myMat)
#    print(recommend(myMat, 2))
#    print(recommend(myMat, 2, simMeas = ecludSim))
#    print(recommend(myMat, 2, simMeas = pearsSim))

#    print(recommend(myMat, 2, estMethod = svdEst))
#    print(recommend(myMat, 2, simMeas = ecludSim, estMethod = svdEst))
#    print(recommend(myMat, 2, simMeas = pearsSim, estMethod = svdEst))

    imgCompress()
    

if __name__ == "__main__":
    easyToUse()
