# coding: utf-8

import scipy as sp
import matplotlib.pyplot as plt
from scipy.optimize import fsolve

# step 1 readData
def loadData():
    data = sp.genfromtxt("../data/web_traffic.tsv", delimiter = '\t')
    print(data[:10])
    print(data.shape)
    return data


# 2 preprocess and clean data
def plotData(x, y):
    plt.scatter(x, y)
    plt.title("web traffic over the last mouth")
    plt.xlabel("Time")
    plt.ylabel("Hits/hour")
    plt.xticks([w*7*24 for w in range(10)], ['week %i' %w for w in range(10)])
    plt.autoscale(tight = True)
    plt.grid()
    plt.show()

# preprecess
def preprocess(data):
    x = data[:, 0]
    y = data[:, 1]
    # 数组无效值判断 sp.isnan(y) 返回 array([true, false, false, ....], dtype = bool)
    print(sp.sum(sp.isnan(y)))
    # 条件索引 x y
    x = x[~sp.isnan(y)]
    y = y[~sp.isnan(y)]
#    plotData(x, y)
    return x, y


def error(f, x, y):
    #f(x) - y 得到ndarray
    # ndarray ** 2 作用在每个元素.
    return sp.sum((f(x) - y)**2)

def plotModel(func, x):
    fx = sp.linspace(0, x[-1], 1000)
    plt.plot(fx, func(fx), linewidth = 3)
    plt.legend(["d = %i" % func.order], loc="upper left")


# step 4 select model
def selectModel(x, y):
    fp1, res, rank, sv, rcond = sp.polyfit(x, y, 1, full=True)
    print("Model paramters is %s" % fp1)
    print(res)
    func1 = sp.poly1d(fp1)
    print(error(func1, x, y))
    plotModel(func1, x)

    fp2 = sp.polyfit(x, y, 2)
    print(fp2)
    func2 = sp.poly1d(fp2)
    print(error(func2, x, y))

# step 5 评估model
# another Sight of Data may be a good direction
def anotherDataSight(x, y):
    inflection = int(3.5*7*24)
    testStart = 700

    xa = x[:inflection]
    ya = y[:inflection]

    xb = x[inflection:testStart]
    yb = y[inflection:testStart]

    testx = x[testStart:]
    testy = y[testStart:]

    fa = sp.poly1d(sp.polyfit(xa, ya, 1))
    fb = sp.poly1d(sp.polyfit(xb, yb, 1))
    fa_error = error(fa, testx, testy)
    fb_error = error(fb, testx, testy)
    print("Error Inflection is %f" %(fa_error + fb_error))

    fb1 = sp.poly1d(sp.polyfit(xb, yb, 1))
    fb2 = sp.poly1d(sp.polyfit(xb, yb, 2))
    fb10 = sp.poly1d(sp.polyfit(xb, yb, 10))
    fb100 = sp.poly1d(sp.polyfit(xb, yb, 100))
    print("Error only use the most importent data :  ", error(fb1, testx, testy))
    print("Error only use the most importent data :  ", error(fb2, testx, testy))
    print("Error only use the most importent data :  ", error(fb10, testx, testy))
    print("Error only use the most importent data :  ", error(fb100, testx, testy))

    print(fb2)
    reach_max = fsolve(fb2-100000, 1000)/(7*24)
    print("100000 Hits/hour expected at week %f" % reach_max)







def easyToUse():
    data = loadData()
    x, y = preprocess(data)
    selectModel(x, y)
    anotherDataSight(x, y)



if __name__ == "__main__":
    easyToUse()

