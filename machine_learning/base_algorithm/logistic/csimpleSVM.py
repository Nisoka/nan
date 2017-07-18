# -*- coding: utf-8 -*-
__author__ = 'liu.junnan'

'''
    learn from 支持向量机-码农场
'''
from numpy import *
import matplotlib.pyplot as plt
from matplotlib.patches import *




def selectJrand(i, m):
    j = i
    while j == i:
        j = int(random.uniform(0, m))
    return j

def clipAlpha(aj, H, L):
    if aj > H:
        aj = H
    elif aj < L:
        aj = L

    return aj


'''
    smoSimple(dataX, labels, C, toler, maxIter, kernelOption = 'linear'):
        dataX
            type: matrix
        labels
            type: matrix
            shape: (entrySum, 1)
'''

def smoSimple(dataX, labels, C, toler, maxIter, kernelOption = 'linear'):
    '''
    简化smo

    C  惩罚参数,
        C越大 惩罚越厉害, 越不能允许有数据点在 间隔内形成软间隔
        因为当 alpha = C 时, 对应的数据点就成为 松弛向量
        因为 sigema 是松弛因子，当松弛因子>0 时,

    toler 容错率

    '''
    dataMat = mat(dataX)
    labelMat = mat(labels).transpose()
    m, n = shape(dataMat)
    alphas = mat( zeros( (m, 1) ) )
    b = 0
    iter = 0

    # 外层循环, 多次循环优化, 尽力优化间隔
    while iter < maxIter:
        alphaPairsChanged = 0

        #step 1: <选择>alphai 遍历循环, 内层循环
        for i in range(m):

            # step 2: <计算> xi 的误差值 Ei
            # vec_w = alpha*y * mat_x     f(xi) = wT * x_i + b
            fXi = float(multiply(alphas, labelMat).T * dataMat * dataMat[i, :].T) + b
            Ei = fXi - float(labelMat[i])

            # step 3: <判断> xi 是否 违背KKT条件,
            #         违背KKT条件的xi 对应的alphai 应该被优化
            ### check and pick up the alpha who violates the KKT condition
            ## satisfy KKT condition
            # 1) yi*f(i) >= 1 and alpha == 0 (outside the boundary)
            # 2) yi*f(i) == 1 and 0<alpha< C (on the boundary)
            # 3) yi*f(i) <= 1 and alpha == C (between the boundary)
            ## violate KKT condition
            # because y[i]*E_i = y[i]*f(i) - y[i]^2 = y[i]*f(i) - 1, so
            # 1) if y[i]*E_i < 0, so yi*f(i) < 1, if alpha < C, violate!(alpha = C will be correct)
            # 2) if y[i]*E_i > 0, so yi*f(i) > 1, if alpha > 0, violate!(alpha = 0 will be correct)
            # 3) if y[i]*E_i = 0, so yi*f(i) = 1, it is on the boundary, needless optimized

            if ((labelMat[i]*Ei < -toler) and (alphas[i] < C)) or\
                ((labelMat[i]*Ei > toler) and (alphas[i] > 0)):

                # step 4: <选择> alphai 对应的 alphaj
                #         计算对应的 Ej
                j = selectJrand(i, m)


                # step 5: <计算> Ei Ej eta H L 等 优化需要的元素
                fXj = float(multiply(alphas, labelMat).T * dataMat * dataMat[j, :].T) + b
                Ej = fXj - float(labelMat[j])
                #       保存alphaI alphaJ 留待使用
                alphaIold = alphas[i].copy()
                alphaJold = alphas[j].copy()
                #        alphaI alphaJ 有上下限 0 - C
                #        并 alphaI + alphaJ = value 是固定的 线性优化。
                if labelMat[i] != labelMat[j]:
                    L = max(0, alphas[j] - alphas[i])
                    H = min(C, C + alphas[j] - alphas[i])
                else:
                    L = max(0, alphas[j] + alphas[i] - C)
                    H = min(C, alphas[j] + alphas[i])
                if L==H:
                    print(" >>>>>>>>>>>>>>>>> L == H")
                    continue
                eta = 2.0 * dataMat[i, :]*dataMat[j, :].T - dataMat[i, :]*dataMat[i, :].T - \
                    dataMat[j, :]*dataMat[j, :].T
                if eta >= 0:
                    print(" >>>>>>>>>>>>>>>> eta  >= 0")
                    continue

                # step 6: <优化> alphaJ alphaI b
                #         并通过H L 限制。
                #         根据alphaJ alphaJold Yi Yj 优化 alphasI
                alphas[j] -= labelMat[j]*(Ei - Ej)/eta
                alphas[j] = clipAlpha(alphas[j], H, L)

                if abs(alphas[j] - alphaJold) < 0.00001:
                    print(" j not move enough")
                    continue

                alphas[i] += labelMat[j] * labelMat[i] * (alphaJold - alphas[j])

                tempEalphai = alphas[i] - alphaIold
                tempEalphaj = alphas[j] - alphaJold
                b1 = b - Ei - labelMat[i]*tempEalphai * dataMat[i, :] * dataMat[i, :].T \
                                     - labelMat[j] * tempEalphaj * dataMat[i, :]*dataMat[j, :].T

                b2 = b - Ej - labelMat[i]*tempEalphai * dataMat[i, :] * dataMat[j, :].T \
                                     - labelMat[j] * tempEalphaj * dataMat[j, :]*dataMat[j, :].T

                if 0 < alphas[i] and C > alphas[i]:
                    b = b1
                elif 0 < alphas[j] and C > alphas[j]:
                    b = b1
                else:
                    b = (b1+b2)/2.0

                #         执行到此，说明 有 alpha 被优化,
                #         因为是梯度下降, 说明还可以继续优化,继续下降
                alphaPairsChanged += 1

        if alphaPairsChanged == 0:
            iter += 1
        else:
            iter = 0

        print("iter number : %d" % iter)

    return b, alphas



def calcWs(alphas, dataX, labels):
    X = mat(dataX)
    labelMat = mat(labels).transpose()
    m, n  = shape(X)
    w = zeros( (n, 1) )
    for i in range(m):
        w += multiply(alphas[i]*labelMat[i], X[i, :].T)
    return w



'''
    plotSvm(dataArr, labelArr, w, b, svList)
    dataArr
        type: matrix
    labelArr
        type: matrix
    svList
        type: list
list 使用[][] 方式访问二维数组 比较方便
    第一个[1] 维度获得了 低维度的元素
    第二个[1] 又获得更低维度

mat ndarray类型访问 不能使用[1][1]方式访问
    [
        [0, 1],
        [0, 1],
        [0, 1],
        [0, 1],
    ]
    当使用 [1][1]访问时,
    第一个1 得到结果
        [[0,1]]
        此时得到的还是 二维ndarray 而没有变为一维
    当此时使用[1][1]第二个1 时,那么上去访问的是高维空间
    此时高维长度只有0元素，所以结果肯定不对了

    只有当使用 多维度索引[1, 1]方式能够得到 想要的结果

'''
def plotSvm(dataArr, labelArr, w, b, svList):
    xcord0 = []
    ycord0 = []
    xcord1 = []
    ycord1 = []
    markers = []
    colors = []

    for i in range(len(labelArr)):
        xPt = dataArr[i, 0]
        yPt = dataArr[i, 1]
        label = labelArr[i]
        if(label == -1):
            xcord0.append(xPt)
            ycord0.append(yPt)
        else:
            xcord1.append(xPt)
            ycord1.append(yPt)

    fig = plt.figure()
    ax = fig.add_subplot(111)
    ax.scatter(xcord0, ycord0, s=90)
    ax.scatter(xcord1, ycord1, s=90, c='red')

    for sv in svList:
        circle = Circle((dataArr[sv, 0], dataArr[sv, 1]), 0.5, \
            edgecolor=(0, 0.8, 0.8), linewidth = 3, alpha=0.5)
        ax.add_patch(circle)

    w0 = w[0][0]
    w1 = w[1][0]
    b = float(b)
    b1 = b + 1
    b2 = b - 1
    x = arange(-2.0, 12.0, 0.1)
    y = (-w0*x - b)/w1
    y1 = (-w0*x - b1)/w1
    y2 = (-w0*x - b2)/w1
    ax.plot(x, y)
    ax.plot(x, y1)
    ax.plot(x, y2)
    ax.axis([-2, 12, -8, 6])
    plt.show()














'''
    1 simpleSVM()
        __init__(self, data_x, data_y, C, toler, maxIter, kernelOption):
        构造 分类器元素

        data_x
        data_y
            type:matrix
            数据集
        C
            乘法因子
        toler
            容错量
        maxIter
            最大优化次数
        kernelOption
            选择使用的核函数

    2 train(self)
        根据训练数据集训练分类器
        生成需要的各种元素

    3 predict(self, entry_x)

        输出 分类器预测 y
        entry_x
            type: matrix
            entry_x 一个具体事例

    4 experienceError(self)
        训练得到 分类器 对训练数据集的经验误差

    5 plotTrainResult(self)
        可视化 绘制训练数据集,以及训练分类器

'''
'''
    一个类,的名字 应该怎么起
'''
class simpleSVM:
    def __init__(self, data_x, data_y, C, toler, maxIter, kernelOption='linear'):
        self.C = C
        self.toler = toler
        self.maxIter = maxIter
        self.kernelOption = kernelOption

        self.data_x = data_x
        self.data_y = data_y

        self.m = data_x.shape[0]
        self.n = data_x.shape[1]
        self.alphas = mat(zeros( (self.m, 1) ))
        self.b = 0
        self.w = mat(zeros( (self.n, 1) ))


    def train(self):
        self.b, self.alphas = smoSimple(self.data_x, self.data_y, self.C, self.toler, self.maxIter, self.kernelOption)
        self.w = calcWs(self.alphas, self.data_x, self.data_y)

    def predict(self, entry_x):
        result = entry_x*self.w + self.b
        return result

    def experienceError(self):
        sum = self.data_x.shape[0]
        cnt = 0
        for i in range(sum):
            temp = self.predict(self.data_x[i])
            result = -1
            if temp > 0:
                result = 1
            if result != self.data_y.T[i]:
                cnt += 1
                print("Error: predict index %d" % (i))

        ret = float(cnt)/float(sum)
        return ret

    def plotTrainResult(self):
        svList = []
        for i in range(len(self.alphas)):
            if abs(self.alphas[i]) > 0.00001:
                svList.append(i)
        plotSvm(self.data_x, self.data_y.T, self.w, self.b, svList)









'''
    easyToUse()
'''
def loadDataSet(fileName):
    fileType = fileName.split('.')[1]
    splitChar = '\t'
    if fileType == 'csv':
        splitChar = ','
    elif fileType == 'txt':
        splitChar = '\t'
    else:
        splitChar = '\t'
    dataMat = []
    labelMat = []
    fr = open(fileName)
    for line in fr.readlines():
        lineArr = line.strip().split(splitChar)
        dataMat.append( [float(lineArr[0]), float(lineArr[1])] )
        labelMat.append( float(lineArr[2]) )
    return dataMat, labelMat

def easyToUse(fileName):
    dataArr, labelArr = loadDataSet(fileName)
    data_x = mat(dataArr)
    data_y = mat(labelArr)

    # 调整 maxIter(5 -- 100)
    #       可明显缩短时间 而对分类器结果影响不大
    # 调整 C (0.05 -- 100)
    #       可明显查看支持向量的变化情况
    svm = simpleSVM(data_x, data_y, 10, 0.001, 20)
    svm.train()

    svm.experienceError()
    svm.plotTrainResult()

    print("W")
    print(svm.w)
    print("b")
    print(svm.b)
    print(" ")


if __name__ == "__main__":
    easyToUse('testSet.txt')

