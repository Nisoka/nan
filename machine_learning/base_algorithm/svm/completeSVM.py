# -*- coding: utf-8 -*-

from numpy import *
import matplotlib.pyplot as plt
from matplotlib.patches import Circle

def loadDataSet(fileName):
    dataMat = []
    labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr = line.strip().split('\t')
        dataMat.append([float(lineArr[0]), float(lineArr[1])])
        labelMat.append(float(lineArr[2]))
    return dataMat, labelMat


def kernelTrans(X, A, kTup):
    m, n = shape(X)
    K = mat(zeros((m, 1)))
    if kTup[0] == 'linear':
        K = X * A.T
    elif kTup[0] == 'rbf':
        for i in range(m):
            deltaRow = X[i, :] - A
            K[i] = deltaRow * deltaRow.T
        K = exp(K/(-1*kTup[1]**2))
    else:
        raise NameError(" the kernel is no recognized!")

    return K

class optStruct:
    def __init__(self, dataMatIn, classLabels, C, toler, kTup):
        self.X = dataMatIn
        self.labelMat = classLabels
        self.C = C
        self.toler = toler
        self.m = shape(dataMatIn)[0]
        self.alphas = mat(zeros((self.m, 1)))
        self.b = 0
        self.eCache = mat(zeros((self.m, 2)))
        self.K = mat(zeros((self.m, self.m)))
        for i in range(self.m):
            self.K[:, i] = kernelTrans(self.X, self.X[i, :], kTup)


def calcEk(oS, k):
    # matrix multiply(a, b) == a[i]*b[i] shape 不变
    # matrix *  == numpy.dot
    fXk = float(multiply(oS.alphas, oS.labelMat).T * oS.K[:, k] + oS.b)
    Ek = fXk - float(oS.labelMat[k])
    return Ek

def clipAlpha(aj, H, L):
    if aj > H:
        aj = H
    if aj < L:
        aj = L
    return aj

def selectJrand(i, m):
    j = i
    while j == i:
        j = int(random.uniform(0, m))
    return j

def selectJ(i, oS, Ei):
    maxK = -1
    maxDeltaE = 0
    Ej = 0

    oS.eCache[i] = [1, Ei]
    validEcacheList = nonzero(oS.eCache[:, 0].A)[0]
    if len(validEcacheList) > 1:
        for k in validEcacheList:
            if k == i:
                continue
            Ek = calcEk(oS, k)
            deltaE = abs(Ei - Ek)
            if deltaE > maxDeltaE:
                maxK = k
                maxDeltaE = deltaE
                Ej = Ek
        return  maxK, Ej
    else:
        j = selectJrand(i , oS.m)
        Ej = calcEk(oS, j)

    return j , Ej

def updateEk(oS, k):
    Ek = calcEk(oS, k)
    oS.eCache[k] = [1, Ek]


def innerL(i , oS):
    Ei = calcEk(oS, i)
    kktValue = oS.labelMat[i]*Ei
    # y(wx+b) - 1 < 0 must ai > C
    # y(wx+b) - 1 > 0 must ai == 0
    # y(wx+b) - 1 = 0 must ai (0 C)
    # step 1: KKT judgement
    if (kktValue < -oS.toler and oS.alphas[i] < oS.C) or \
            (kktValue > oS.toler and oS.alphas[i] > 0):

        # step 2: select alphaJ
        j, Ej = selectJ(i, oS, Ei)

        alphaIold = oS.alphas[i].copy()
        alphaJold = oS.alphas[j].copy()

        # element L H   for alphaI alphaJ
        if oS.labelMat[i] != oS.labelMat[j]:
            L = max(0, oS.alphas[j] - oS.alphas[i])
            H = min(oS.C , oS.C + oS.alphas[j] - oS.alphas[i])
        else:
            L = max(0, oS.alphas[j] + oS.alphas[i] - oS.C)
            H = min(oS.C, oS.alphas[j] + oS.alphas[i])
        if L == H:
            print "L == H continue next alphai"
            return 0

        # element eta
        eta = 2.0 * oS.K[i, j] - oS.K[i, i] - oS.K[j, j]

        if eta >= 0:
            print "eta >= 0 continue next alphai"
            return 0
        # step 3: optimize alpha and judget if move enough
        oS.alphas[j] -= oS.labelMat[j]*(Ei - Ej)/eta
        oS.alphas[j] = clipAlpha(oS.alphas[j], H, L)
        updateEk(oS, j)
        if abs(oS.alphas[j] - alphaJold) < 0.00001:
            print "alpha j not optimize enough continue next alphai"
            return 0

        oS.alphas[i] += oS.labelMat[j]*oS.labelMat[i]*(alphaJold - oS.alphas[j])
        updateEk(oS, i)

        #step 4: update b
        alphaIoptimize = oS.alphas[i] - alphaIold
        alphaJoptimize = oS.alphas[j] - alphaJold
        b1 = oS.b - Ei - oS.labelMat[i]*alphaIoptimize*oS.K[i, i] - \
             oS.labelMat[j] * alphaJoptimize * oS.K[i, j]
        b2 = oS.b - Ej - oS.labelMat[i]*alphaIoptimize*oS.K[i, j] - \
             oS.labelMat[j] * alphaJoptimize * oS.K[j, j]

        if 0 < oS.alphas[i] and oS.C > oS.alphas[i]:
            oS.b = b1
        elif 0< oS.alphas[j] and oS.C > oS.alphas[j]:
            oS.b = b2
        else:
            oS.b = (b1 + b2)/2.0

        return 1
    else:
        #is in KKT
        return 0

def smoP(dataMatIn, classLabels, C, toler, maxIter, kTup = ('linear', 0)):
    oS = optStruct(mat(dataMatIn), mat(classLabels).transpose(), C, toler, kTup)
    iter = 0
    entireSet = True
    alphaPairsChanged = 0
    while (iter < maxIter) and (alphaPairsChanged > 0 or entireSet):
        alphaPairsChanged = 0
        if entireSet:
            for i in range(oS.m):
                alphaPairsChanged += innerL(i, oS)
            iter += 1
        else:
            # alpha if in (0, C)  is sup vector can optimize
            nonBoundIs = nonzero((oS.alphas.A > 0) * (oS.alphas.A < C))[0]
            for i in nonBoundIs:
                alphaPairsChanged += innerL(i, oS)
            iter += 1

        if entireSet:
            entireSet = False
        elif alphaPairsChanged == 0:
            entireSet = True

        print "iter now is %d" % iter
    return oS.b, oS.alphas

def calcWs(alphas, dataArr, classLabels):
    X = mat(dataArr)
    labelMat = mat(classLabels).transpose()
    m, n = shape(X)
    w = zeros((n, 1))
    for i in range(m):
        w += multiply(alphas[i]*labelMat[i], X[i, :].T)
    return w


########################
'''
    test the complete SVM
'''
########################

def testRbf(k1 = 1.3):
    dataArr, labelArr = loadDataSet('testSetRBF.txt')
    b, alphas = smoP(dataArr, labelArr, 200, 0.0001, 1000, ('rbf', k1))
    dataMat = mat(dataArr)
    labelMat = mat(labelArr).transpose()
    svInd = nonzero(alphas.A > 0)[0]
    sVs = dataMat[svInd]
    labelSV = labelMat[svInd]
    print "there are %d support Vectors" % shape(sVs)[0]

    m, n = shape(dataMat)
    errorCnt = 0
    for i in range(m):
        kernelEval = kernelTrans(sVs, dataMat[i, :],('rbf', k1))
        predict = kernelEval.T * multiply(labelSV, alphas[svInd]) + b
        if sign(predict) != sign(labelArr[i]):
            errorCnt += 1
    print "the training error rate is %f" %(float(errorCnt)/m)

    # plot
    fig = plt.figure()
    ax = fig.add_subplot(1,1,1)
    x1 = []
    x2 = []
    y1 = []
    y2 = []
    xV = []
    yV = []
    for i in range(m):
        if sign(labelArr[i]) == sign(1):
            x1.append(dataArr[i][0])
            y1.append(dataArr[i][1])
        else:
            x2.append(dataArr[i][0])
            y2.append(dataArr[i][1])
        if alphas[i] != 0:
            xV.append(dataArr[i][0])
            yV.append(dataArr[i][1])

    ax.scatter(x1, y1, c='red')
    plt.scatter(x2, y2, c='blue')
    for i in range(len(xV)):
        circle = Circle((xV[i], yV[i]), 0.05, facecolor='none', edgecolor=(0, 0.8, 0.8), linewidth=1, alpha=0.8)
        ax.add_patch(circle)
    plt.show()






if __name__ == "__main__":
    testRbf()



















