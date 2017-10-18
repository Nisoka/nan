# encoding=utf-8
# @Author: liu.junnan
# @Date:   17-05-18

'''
 list 保存数据比较耗费 内存 并且 因为是保存地址指针，间接引用,也比较耗费CPU
 ndarray 和 标准Python库类array.array并不相同，后者只处理一维数组和提供少量功能

 因此 numpy 提供了 理想的用于数值运算的就是ndarray（N 维 数组） ， 以及 ndarray 的函数功能ufunc
    ndarray 存储单一数据类型的多维数组。
    ufunc 对数组进行遍历处理的函数。
'''


from numpy import *
from numpy.linalg import *

# 1 基础
def ndarrayBase():
    '''
    ndarray 两部分组成
        1 实际数据
        2 描述数据的 元数据
        大部分操作 只 针对元数据，并不改变底层 实际数据
        特点：
            下标 0 开始
            元素统一类型、
            通过一个 正整数元组 索引

        ndarray 类的属性
            rank 秩 -- 维度，轴的数量
            axis 轴 -- 内部 线性方向
            ndarray.ndim    数组的维度 -- 秩
            ndarray.shape   数组的形状 是一个元组 tuple.(最高维, 次高维, ... 最低维)
            ndarray.size    数组元素的个数
            ndarray.dtype   元素的类型(可以设置)
            ndarray.itemsize  元素的字节大小
            ndarray.data    实际数据的缓冲区，通常不使用，都是使用索引来访问元素。
            ndarray.flat    所有元素迭代器

    '''
    '''
        reshape() 直接将ndarray 改变形状
        ndarray 先拉伸为 一维数组， 然后从低维 到 高维 化形。
    '''

    a = arange(15).reshape(3, 5)
    print(a)
    print(a.shape)
    print(a.ndim)
    print(a.size)
    print(a.dtype)
    print(a.data)
    type(a)



def createFunc():
    '''
    1 通过python的 序列对象(list tuple set) 构造ndarray
        类型 是通过原对象 推导得来
        不能 直接传递几个值 代替序列对象。
        ERROR array(1, 2, 3)
        >>> a = array(1,2,3,4)    # WRONG
        >>> a = array([1,2,3,4])  # RIGHT

    '''
    a = array( [1, 2, 3] )
    print(a.dtype)
    type(a)
    b = array( [1.1, 2.2, 3.3] )
    print(b.dtype)

    '''
    2 序列对象 内嵌 序列对象，则创建 高维ndarray
        可以再创建数组时 指定类型dtype
    '''
    a2 = array( [(1, 2, 3), (2, 4, 6)] )
    print(a2)
    b2 = array( [(1, 2, 3), (2, 4, 6)], dtype = complex )
    print(b2)

    '''
    3 占位符方式快速创建 ndarray
        通常数组元素未知, shape 形状已知。
        zeros  ones empty
        默认类型 dtype 是 float64
    '''
    a3 = ones( (2, 3), dtype = int16)
    print(a3)
    b3 = zeros( (2, 3) ) # dtype = float
    print(b3)

    '''
    4 数列方式创建 ndarray
        arange(end)
        # array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        # 共10个，间隔1
        创建从 0 到 end 间隔为1 的ndarray
            0, 1, ..... end-2, end-1
            不含有 end

        arange(start, end, spaceValue)
        # array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9])
        # 共 (end - start) / space 个
        创建从 start 到 end 间隔为 space 的ndarray。
            不好预测 ndarray总数
            不含有 end

        linspace(start, end, cnt)
        # array([0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
        # 共 11 个 ， 间隔为(end+1-start)/cnt
        创建从 start 到 end 以cnt 为总数，自动分割间隔 的ndarray,
            含有 end

    其它函数array, zeros, zeros_like, ones, ones_like, empty,
        empty_like, arange, linspace, rand, randn, fromfunction, fromfile  参考： NumPy示例

    '''

    a4 = arange(10)
    a41 = arange(0, 10, 1)   #space = 1
    b4 = linspace(0, 10, 11)     #cnt = 11
    print(a4, a41, b4)

    '''
    5 打印ndarray
        嵌套列表的形式打印ndarray
        实际ndarray 就是高维数组
        一维数组被打印成行，二维数组成矩阵，三维数组成矩阵列表。


        如果一个数组用来打印太大了，NumPy自动省略中间部分而只打印角落
        >>> print arange(10000)
        [   0    1    2 ..., 9997 9998 9999]
        禁用NumPy的这种行为并强制打印整个数组，你可以设置printoptions参数来更改打印选项。
        >>> set_printoptions(threshold='nan')

    注意
        >>> arange(4)
        array([0, 1, 2, 3])
            目的是查看变量 类似 watch的功能，
            所以会显示类型,并且显示','
        >>> a = arange(4)
        >>> print(a)
        [0 1 2 3]
            print ndarray 时,不显示',' 不显示 array()

    '''
    a = arange(12)
    a2 = arange(12).reshape(3, 4)
    a3 = arange(24).reshape(2, 3, 4)
    print(a)
    print(a2)
    print(a3)




'''
   ndarray 的算数运算

'''
def calFunc():

    '''
    1 基本运算符 + - * / % ** 等
        1 遍历进行算数运算得到结果
        2 用结果创建新ndarray

    '''
    print("算数运算 基本 + - * / ** ")
    a = linspace(0, 9, 10)
    b = arange(10)
    print(a, b)
    print(a-b)
    print(a*b)  #矩阵点积，则需要使用 dot 函数计算。
    print(a**2)
#    print(10*sin(a))       ???
    print(a<5)


    '''
    2 += *= 自变操作符
       用来改变已存在数组，并不创建新数组
    '''
    print("算数运算 += ")
    print(a)
    a+=10
    print(a)

    print(a)
    a+=b
    print(a)

    '''
    3  ndarray 类内方法
        1 计算所有元素和
        2 按维度axis 计算元素和
            axis = 0 - ndim(秩)
            0 代表最高维
    '''
    print("算数运算 类内方法 max min sum ")
    a = arange(24).reshape( (2, 3, 4) )
    print(a)
    sum = a.sum()
    min = a.min()
    max = a.max()
    print(sum)
    print(min)
    print(max)



    print("算数运算 类内方法 max min sum  ==  axis ")
    sum = a.sum(axis = 2) # 低维取sum
    print(sum)
    sum = a.sum(axis = 1) # 次低维取sum
    print(sum)
    sum = a.sum(axis = 0) # 高维取sum
    print(sum)

    min = a.min(axis = 1)
    max = a.max(axis = 2)
    print(min)
    print(max)

    cumsum = a.cumsum(axis = 2)
    print(cumsum)



'''
    numpy 提供基本的数学函数 sin exp 等
    叫做 通用函数 ufunc()
    使用方法类似 算术运算符 逐个元素运算。

    更多函数all, alltrue, any, apply along axis, argmax, argmin,
        argsort, average, bincount, ceil, clip, conj, conjugate,
        corrcoef, cov, cross, cumprod, cumsum, diff, dot, floor,
        inner, inv, lexsort, max, maximum, mean, median, min, minimum,
        nonzero, outer, prod, re, round, sometrue, sort, std, sum,
        trace, transpose, var, vdot, vectorize, where
    参见: NumPy示例
'''
def ufunc():
    '''

    '''
    print("u functions ")

    a = arange(3)
    res = exp(a)
    print(res)
    res = sqrt(a)
    print(res)
    b = arange(10, 13, 1)
    res = add(a, b)
    print(res)










'''
    索引 切片 迭代

    索引
        [i][j]
        [i, j]

    切片
        1 [start:end, start2:end2]

        2 [start:end:space, start:end:space]


'''
def sliceFunc():
    print("splitIndex ==== ")
    a = arange(12).reshape(3, 4)
    print(a)


    res = a[1, 1]  # 正常索引 直接按坐标索引
    print(res)

    res = a[1][1]  # 非正常索引, 索引高维，然后索引低维
    print(res)


    res = a[0:2, 0:2]    # 正常切片(不包含end)
    print(res)
    res = a[0:2, ]  #正常切片，只切片了高维，低维直接保留
    print(res)

    res = a[0:2][0:2]     # ERROR 不正确不是切片，不能索引
    print(res)

    #    间隔切片
    res = a[0:2:1, 0:4:2]
    print(res)
    b = arange(10)
    print(b)
    res = b[::-1]       #直接翻转
    print(res)
    res = b[0:8:-1]     #ERROR  不能切片后使用-1 间隔切片
    print(res)

    '''
    点 (…)代表许多产生一个完整的索引元组必要的分号。
    如果x是秩为5的数组(即它有5个轴)，那么:
      ● x[1,2,…] 等同于 x[1,2,:,:,:],
      ● x[…,3] 等同于 x[:,:,:,:,3]
      ● x[4,…,5,:] 等同 x[4,:,:,5,:].

    '''
    c = arange(120).reshape( (int(120/3), 3) )
    print(c.shape)
    #    (2, 2, 3)
    print(c[1,...])
    # same as c[1,:,:] or c[1] array([[100, 101, 102],        [110, 112, 113]])
    print(c[...,2])
    # same as c[:,:,2] array([[  2,  13],        [102, 113]])



#    迭代
#    ndarray 中的flat 属性 是一个迭代器，迭代所有元素。
#    for element in c.flat:
#        print(element,)



'''
    ndarray 的形状shape 就是每个维度轴上元素个数
'''
def shapeFunc():
    print("ndarray shape function ... ")
#    a = floor(10*random.random(3,4))
    a = arange(12).reshape(3, 4)
    print(a)
    print(a.shape)

    '''
    修改形状方法:
        这些都是 浅复制，都是视图， 数据还是同一数据
       0 reshape()
         返回新ndarray，原ndarray不变。
       1 ravel() flatten the ndarray
         低维展开ndarray,
         返回新ndarray，原ndarray不变。
       2 a.shape = (m, n)
         设置 shape属性改变ndarray形状
         直接改变原ndarray
       3 a.transpose()
         转置ndarray
         返回新ndarray，原ndarray不变。

       如果在改变形状操作中一个维度被给做-1，其维度将自动被计算
    '''
    print("ravel() ")
    b = a.ravel()
    print(a)
    print(b)
    print("shape = ")
    a.shape = (6, 2)
    print(a)


    print("resize")
    a.resize( (2, 6) )
    print(a)

    print("reshape")
    b = a.reshape( (6, 2) )
    print(a)
    print(b)
    # 自动计算 -1 的大小
    c = b.reshape( (3,-1) )
    print(c)

    print("transpose")
    b = a.transpose()
    print(a)
    print(b)


'''
    使用 help(name)
    来查看 name 函数、类、module 的帮助信息
'''


'''
    组合两个ndarray
'''
def stackFunc():
    "stack the ndarrays : c = a append b"
    print("stack a and b === ")
    a = arange(0, 8, 1).reshape((2, 2, 2))
    b = linspace(10, 17, 8).reshape( (2, 2, 2) )
    print(a)
    print(b)

    '''
        按不同轴 组合 ndarray
        1 stack 函数族  按方向组合
            c = stack(a, b)
        常用函数 vstack( (a, b) )
            参数是一个tuple 元组
        vstack 按最高维进行组合 (对于二维的就是 行增加 axis = ??)
        hstack 按次高维进行组合 (对于二维的就是 列增加 axis = ??)

        对那些维度比二维更高的数组，
            vstack 沿着第一个轴组合,
            hstack 沿着第二个轴组合，
        还有函数row_stack column_stack   类似vstack hstack

        2 append 函数  按给定axis 组合
            axis = 0 最高维度， 1 2 依次降低
            c = append(a, b, axis = n)
            axis 不能省略，不然就是展开成为了 一维数组


    '''
    print("vstack -- ")
    c = vstack((a, b))
#    print(a)
    print(c)
    print("hstack -- ")
    c = hstack((a, b))
    print(c)


    print("stack a and b ===  shape 2, 2")
    a = arange(0, 4, 1).reshape((2, 2))
    b = linspace(10, 13, 4).reshape( (2, 2) )
    print(a)
    print(b)

    print("vstack -- ")
    c = vstack((a, b))
#    print(a)
    print(c)
    print("hstack -- ")
    c = hstack((a, b))
    print(c)


    '''
        append(a, b, axis = n)
        axis = 0 最高维度
        axis = 1, 2, 3 逐次降维
    '''

    print("append 组合两个数组")
    a = arange(12).reshape(-1,3)
    b = arange(6).reshape(-1,3)
    print(a)
    print(b)
    c = append(a, b, axis = 0)
    print(c)

    a = arange(12).reshape(3,-1)
    b = arange(6).reshape(3,-1)
    print(a)
    print(b)
    c = append(a, b, axis = 1)
    print(c)








'''
    分割ndarray
    split(a, rule, axis = index)
        axis = 0 最高维度
        axis = 1,2,3 逐次降维

    vsplit(a, rule)
        最高维
    hsplit(a, rule)
        次高维


    rule :
        rule 是一个值时，将a整分几分
        rule 是一个元组(x, y, z ... )时，将a 在 axis 方向上，按 0-x, x-y, y-z .. 分割
'''
def splitFunc():
    a  = arange(36).reshape(2, 3, 6)
    print(a)


    b = split(a, 2, axis = 0)
    print(b)
    b = split(a, 3, axis = 1)
    print(b)
    b = split(a, 2, axis = 2)
    print(b)


'''
    python 是指针基础语言，所有对象默认都是指针引用方式。
        所以变量名不含有任何类型，实际类型是在指针指向内存中保存。
        这就是动态类型的实质。
        C 中的指针对象。

    在函数调用时 发生的对象拷贝，也是引用拷贝，所以不要觉得在函数中随意修改数据没关系
        而是将内存拷贝，生成了新的对象。
        C 中的指针传递。

    id(obj)    id 能独一无二的标识 一个对象


'''
def testFuncParam(x):
    return id(x)
'''
    ndarray 的复制
    1 赋值   不复制
        python 动态类型实质 -- 指针本质
    2 浅复制 不复制，
        新的查看方式、方法？就是一个新的视图，但是实际上还是一个数据缓冲区 a.data
        不同数组 共享同一数据 a.data
        视图方法创建的 新ndarray 和 原ndarray 都是同一个数据
        修改新ndarray 两个都修改
        实际未复制数据，但是复制了新的形状等参数。

    3 深复制
'''
def copyFunc():
    '''
        1 赋值 不复制
    '''
    print("1 赋值 不复制")
    a = arange(12).reshape(2, 6)
    b = a
    print( b is a)

    b.shape = (3, 4)
    print(a.shape)

    print("函数参数 是否是新的对象？")
    res = testFuncParam(a)
    print(id(a))
    print(res)

    '''
        2 视图 浅复制 --- 实际未复制数据，但是复制了新的形状等参数。
            1 a.view()
            2 slice 切片
    '''
    print("2 视图 浅复制， 未复制")
    b = a.view()
    print( b is a )
    print( b.base is a )
    print( b.flags.owndata )
    b[0, 1] = 1234
    print(a)

    b.shape = (3, 4)
    print(a)
    print(b)

    c = a[:, 0:3]
    print(c)
    c[0, 0] = 111
    print(a)

    '''
        3 深复制 -- 真复制
            copy()
    '''
    print("2 深复制  真复制")
    b = a.copy()
    print( b is a )
    print( b.base is a )
    b[0 , 0] = 2222
    print(a)
    print(b)





'''
    函数和方法(method)总览
        这是个NumPy函数和方法分类排列目录。
    这些名字链接到 NumPy示例 ,你可以看到这些函数起作用。[^5]
    创建数组
    arange, array, copy, empty, empty_like, eye, fromfile, fromfunction, identity, linspace, logspace, mgrid, ogrid, ones, ones_like, r , zeros, zeros_like
    转化
    astype, atleast 1d, atleast 2d, atleast 3d, mat
    操作
    array split, column stack, concatenate, diagonal, dsplit, dstack, hsplit, hstack, item, newaxis, ravel, repeat, reshape, resize, squeeze, swapaxes, take, transpose, vsplit, vstack
    询问
    all, any, nonzero, where
    排序
    argmax, argmin, argsort, max, min, ptp, searchsorted, sort
    运算
    choose, compress, cumprod, cumsum, inner, fill, imag, prod, put, putmask, real, sum
    基本统计
    cov, mean, std, var
    基本线性代数
    cross, dot, outer, svd, vdot
'''










'''
    高级技巧！！！！
'''
'''
    进阶
    广播法则(rule)
    广播法则能   使通用函数有意义地处理不具有相同形状的输入。
    广播第一法则是, 如果所有的输入数组维度不都相同，
        一个“1”将被重复地添加在维度较小的数组上直至所有的数组拥有一样的维度。
    广播第二法则, 确定长度为1的数组沿着特殊的方向表现地好像它有沿着那个方向最大形状的大小。
        对数组来说，沿着那个维度的数组元素的值理应相同。

    应用广播法则之后，所有数组的大小必须匹配。更多细节可以从这个 文档 找到。
'''

'''
    高级索引
    Numpy 的 ndarray 比 普通的序列对象 提供更多的索引功能。
    1 可以被整数数组 或 布尔数组索引
'''
def advanceIndex():
    '''
        1 ndarray 利用数组索引
    '''
    a = arange(12)**2
    print(a)
    print("ndarray 利用 数组 索引 数组")
    i = array([1, 1, 2, 5, 9])
    b = a[i]
    print(b)

    j = array([[1,2], [3,4]])
    b = a[j]
    print(b)

    '''
        当被索引数组a是多维的时，
            每一个唯一的索引数列 指向a的第一维 -- 最高维
                (就像索引普通数组，不过数组元素还是数组)
        以下示例通过将图片标签用调色版转换成色彩图像展示了这种行为。
    '''

    '''
        palette 调色板 是高维数组, img 图片实际保存的数据是每个位置的颜色索引。
        实际上是保存的palette调色板的索引，
            实际上每次显示图片都是 从新在每个位置刷上颜色，而颜色是通过保存的索引找到的
        每次都重新绘制一下的感觉。
    '''
    print("    被索引数组 a 是多维数组 ")
    palette = array( [ [0,0,0],
                    [255,0,0],
                    [0,255,0],
                    [0,0,255],
                    [255,255,255] ] )
    print(palette)

    img = array([[0, 1, 2, 0],
                [3, 2, 1, 0]])
    imgShowColorArray = palette[img]
    print(imgShowColorArray)

    '''
        也可以给出不不止一维的索引，
        注意：
            每一维的索引数组必须有相同的形状。
    '''
    print("    被索引数组 a 是多维数组 ，多维索引")
    a = arange(12).reshape(3, 4)
    print(a)
    # 索引必须形状shape 相同
    i = array([[0, 1],
                [1, 2]])
    j = array([[2, 1],
                [3, 3]])
    b = a[i, j]
    print(b)
    b = a[i, 2]
    print(b)

    #    自然，我们可以把i和j放到 序列中(比如说列表，但是不能是ndarray中)然后通过list索引。
    print("    list 保存多个维度索引值 索引数组")
    list = [i,j]
    b = a[list]
    print(b)


    '''
    另一个常用的数组索引用法是搜索时间序列最大值 6 。
    >>> time = linspace(20, 145, 5)                 # time scale
    >>> data = sin(arange(20)).reshape(5,4)         # 4 time-dependent series
    >>> time
    array([  20.  ,   51.25,   82.5 ,  113.75,  145.  ])
    >>> data
    array([[ 0.        ,  0.84147098,  0.90929743,  0.14112001],
           [-0.7568025 , -0.95892427, -0.2794155 ,  0.6569866 ],
           [ 0.98935825,  0.41211849, -0.54402111, -0.99999021],
           [-0.53657292,  0.42016704,  0.99060736,  0.65028784],
           [-0.28790332, -0.96139749, -0.75098725,  0.14987721]])
    >>>
    >>> ind = data.argmax(axis=0)                   # index of the maxima for each series
    >>> ind
    array([2, 0, 3, 1])
    >>>
    >>> time_max = time[ ind]                       # times corresponding to the maxima
    >>>
    >>> data_max = data[ind, xrange(data.shape[1])] # => data[ind[0],0], data[ind[1],1]...
    >>>
    >>> time_max
    array([  82.5 ,   20.  ,  113.75,   51.25])
    >>> data_max
    array([ 0.98935825,  0.84147098,  0.99060736,  0.6569866 ])
    >>>
    >>> all(data_max == data.max(axis=0))
    True
    你也可以使用数组索引作为目标来赋值：
    >>> a = arange(5)
    >>> a
    array([0, 1, 2, 3, 4])

    >>> a[[1,3,4]] = 0
    >>> a
    array([0, 0, 2, 0, 0])

    然而，当一个索引列表包含重复时，赋值被多次完成，保留最后的值：
    >>> a = arange(5)
    >>> a[[0,0,2]]=[1,2,3]
    >>> a
    array([2, 1, 3, 3, 4])
    这足够合理，但是小心如果你想用Python的 += 结构，可能结果并非你所期望：
    >>> a = arange(5)
    >>> a[[0,0,2]]+=1
    >>> a
    array([1, 1, 3, 3, 4])
    即使0在索引列表中出现两次，索引为0的元素仅仅增加一次。这是因为Python要求 a+=1 和 a=a+1 等同。


    通过布尔数组索引
    当我们使用整数数组索引数组时，我们提供一个索引列表去选择。
        通过布尔数组索引的方法是不同的, 我们显式地选择数组中我们想要和不想要的元素。
        我们能想到的使用布尔数组的索引最自然方式就是使用和原数组一样形状的布尔数组。
    >>> a = arange(12).reshape(3,4)
    >>> b = a > 4
    >>> b                                          # b is a boolean with a's shape
    array([[False, False, False, False],
           [False, True, True, True],
           [True, True, True, True]], dtype=bool)
    >>> a[b]                                       # 1d array with the selected elements
    array([ 5,  6,  7,  8,  9, 10, 11])
    这个属性在赋值时非常有用：
    >>> a[b] = 0                                   # All elements of 'a' higher than 4 become 0
    >>> a
    array([[0, 1, 2, 3],
           [4, 0, 0, 0],
           [0, 0, 0, 0]])
    你可以参考 曼德博集合示例 看看如何使用布尔索引来生成曼德博集合的图像。
    第二种通过布尔来索引的方法更近似于整数索引；对数组的每个维度我们给一个一维布尔数组来选择我们想要的切片。
    >>> a = arange(12).reshape(3,4)
    >>> b1 = array([False,True,True])             # first dim selection
    >>> b2 = array([True,False,True,False])       # second dim selection
    >>>
    >>> a[b1,:]                                   # selecting rows
    array([[ 4,  5,  6,  7],
           [ 8,  9, 10, 11]])
    >>>
    >>> a[b1]                                     # same thing
    array([[ 4,  5,  6,  7],
           [ 8,  9, 10, 11]])
    >>>
    >>> a[:,b2]                                   # selecting columns
    array([[ 0,  2],
           [ 4,  6],
           [ 8, 10]])
    >>>
    >>> a[b1,b2]                                  # a weird thing to do
    array([ 4, 10])
    注意一维数组的长度必须和你想要切片的维度或轴的长度一致，在之前的例子中，b1是一个秩为1长度为三的数组(a的行数)，b2(长度为4)与a的第二秩(列)相一致。 7
    ix_()函数
    ix_ 函数可以为了获得 多元组 的结果而用来结合不同向量。例如，如果你想要用所有向量a、b和c元素组成的三元组来计算 a+b*c ：
    >>> a = array([2,3,4,5])
    >>> b = array([8,5,4])
    >>> c = array([5,4,6,8,3])
    >>> ax,bx,cx = ix_(a,b,c)
    >>> ax
    array([[[2]],

           [[3]],

           [[4]],

           [[5]]])
    >>> bx
    array([[[8],
            [5],
            [4]]])
    >>> cx
    array([[[5, 4, 6, 8, 3]]])

    >>> ax.shape, bx.shape, cx.shape
    ((4, 1, 1), (1, 3, 1), (1, 1, 5))

    >>> result = ax+bx*cx
    >>> result
    array([[[42, 34, 50, 66, 26],
            [27, 22, 32, 42, 17],
            [22, 18, 26, 34, 14]],
           [[43, 35, 51, 67, 27],
            [28, 23, 33, 43, 18],
            [23, 19, 27, 35, 15]],
           [[44, 36, 52, 68, 28],
            [29, 24, 34, 44, 19],
            [24, 20, 28, 36, 16]],
           [[45, 37, 53, 69, 29],
            [30, 25, 35, 45, 20],
            [25, 21, 29, 37, 17]]])
    >>> result[3,2,4]
    17
    >>> a[3]+b[2]*c[4]
    17
    你也可以实行如下简化：
    def ufunc_reduce(ufct, *vectors):
        vs = ix_(*vectors)
        r = ufct.identity
        for v in vs:
            r = ufct(r,v)
        return r
    然后这样使用它：
    >>> ufunc_reduce(add,a,b,c)
    array([[[15, 14, 16, 18, 13],
            [12, 11, 13, 15, 10],
            [11, 10, 12, 14,  9]],
           [[16, 15, 17, 19, 14],
            [13, 12, 14, 16, 11],
            [12, 11, 13, 15, 10]],
           [[17, 16, 18, 20, 15],
            [14, 13, 15, 17, 12],
            [13, 12, 14, 16, 11]],
           [[18, 17, 19, 21, 16],
            [15, 14, 16, 18, 13],
            [14, 13, 15, 17, 12]]])
    这个reduce与ufunc.reduce(比如说add.reduce)相比的优势在于它利用了广播法则，避免了创建一个输出大小乘以向量个数的参数数组。 8



    用字符串索引
    参见 RecordArray 。
    '''


'''
    from numpy.linalg import *
'''
def linearAlgebra():

    a = arange(4).reshape( (2, 2) )

    '''
        转置
        ndarray的转置 也是很特殊的

        1 对于普通ndarray
            transpose()
        2 对于 行向量
            eg
            [1, 2, 3]
            因为其 shape 是 (3) 秩是1 只有一个维度，是不能转置的
            所以对其进行转置 要特殊进行。
    '''
    print("Normal transpose")
    print(a)
    a.transpose()
    print(a)

    print("line ndarray transpose")
    arr = arange(2)
    print(arr)
    print(arr.shape)
    print(arr.transpose())
    arr.shape = (1, 2)
    print(arr.shape)
    print(arr.transpose())

    '''
    inv 逆矩阵
        1 A 是 n阶方阵
        2 若存在另一个n阶方阵B   AB = BA = E (E是单位阵)

        则 A B 是可逆矩阵, B是A的逆矩阵，
        A 的 逆矩阵 记做A-1

      特点：
        1 逆矩阵唯一性
        2 可逆矩阵一定是方阵
        3 可逆矩阵的转置矩阵  也可逆
        4 可逆矩阵的乘积      也可逆
    '''
    print("inv")
    print(a)
    b = inv(a)
    print(b)

    '''
        单位矩阵 eye represent 'I'
        eye(dim)
    '''
    print("unit")
    u = eye(2)
    print(u)

    '''
        内积 点乘  dot()
    '''
    print("dot")
    print(a)
    b = dot(a, a)
    print(b)

    '''
        trace 求 ndarray的迹(dim 维度)
    '''
    print("trace")
    print(a)
    print(trace(a))

    '''
        solve 求解方程组
        w*x = y
        solve(w, y) 就能够求解出 w*x = y方程组的x的解
        solve 就是 dot 的逆运算
    '''
    print("solve")
    w = arange(4).reshape( (2, 2) )
    x = linspace(13,25,2)
    print(w)
    print(x)
    y = dot(w, x)
    print(y)
    print(" solve now")
    x = solve(w, y)
    print(x)

'''
>>> eig(j)
(array([ 0.+1.j,  0.-1.j]),
array([[ 0.70710678+0.j,  0.70710678+0.j],
       [ 0.00000000-0.70710678j,  0.00000000+0.70710678j]]))
Parameters:
    square matrix

Returns
    The eigenvalues, each repeated according to its multiplicity.

    The normalized (unit "length") eigenvectors, such that the
    column ``v[:,i]`` is the eigenvector corresponding to the
    eigenvalue ``w[i]`` .
'''

'''
    Matrix 直接矩阵类
    并不是ndarray
    是 numpy 模块提供的另外一个类，专用于矩阵计算，
        实际上 ndarray 的二维数组 完全可以替代matrix
'''
def matrixFunc():
    A = matrix('1.0 2.0; 3.0 4.0')
    print(A)
    print("type(A)")
    print(type(A))
    print("A.T")
    print(A.T)

    print("Matrix T 矩阵类的转置 很简单 直接返回T 属性就是 转置矩阵")
    X = matrix('5.0 7.0')
    Y = X.T
    print(Y)

    print("Matrix *  就是矩阵的点乘，不像是ndarray")
    Z = A*Y
    print(Z)

    print("Matrix inv  直接 X.I 就是求矩阵的 逆矩阵！！！！")
    print(A.I)

    print("Matrix solve 还是要使用函数方式")
    print(Y)
    X = solve(A, Z)
    print(X)


'''
索引：比较矩阵matrix 和二维数组 ndarray
    注意NumPy中数组和矩阵有些重要的区别。
NumPy提供了两个基本的对象：一个N维数组对象和一个通用函数对象。
其它对象都是建构在它们之上 的。
    特别的，matrix
    矩阵是继承自NumPy数组对象的二维数组对象。
    对数组和矩阵，索引都必须包含合适的一个或多个这些组合：
        整数标量、省略号 (ellipses)、整数列表,布尔值，整数或布尔值构成的元组，
        和一个一维整数或布尔值数组。
    矩阵可以被用作矩阵的索引，但是通常需要数组、列表或者 其它形式来完成这个任务。

    像平常在Python中一样，索引是从0开始的。
    传统上我们用矩形的行和列表示一个二维数组或矩阵，
    其中沿着0轴的方向被穿过的称作行，沿着1轴的方向被穿过的是列





/*
让我们创建数组和矩阵用来切片：
>>> A = arange(12)
>>> A
array([ 0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11])
>>> A.shape = (3,4)
>>> M = mat(A.copy())
>>> print type(A),"  ",type(M)

>>> print A
[[ 0  1  2  3]
 [ 4  5  6  7]
 [ 8  9 10 11]]
>>> print M
[[ 0  1  2  3]
 [ 4  5  6  7]
 [ 8  9 10 11]]
现在，让我们简单的切几片。基本的切片使用切片对象或整数。例如， A[:] 和 M[:] 的求值将表现得和Python索引很相似。然而要注意很重要的一点就是NumPy切片数组 不 创建数据的副本;切片提供统一数据的视图。
>>> print A[:]; print A[:].shape
[[ 0  1  2  3]
 [ 4  5  6  7]
 [ 8  9 10 11]]
(3, 4)
>>> print M[:]; print M[:].shape
[[ 0  1  2  3]
 [ 4  5  6  7]
 [ 8  9 10 11]]
(3, 4)
现在有些和Python索引不同的了：你可以同时使用逗号分割索引来沿着多个轴索引。
>>> print A[:,1]; print A[:,1].shape
[1 5 9]
(3,)
>>> print M[:,1]; print M[:,1].shape
[[1]
 [5]
 [9]]
(3, 1)
注意最后两个结果的不同。对二维数组使用一个冒号产生一个一维数组，然而矩阵产生了一个二维矩阵。 10 例如，一个 M[2,:] 切片产生了一个形状为(1,4)的矩阵，相比之下，一个数组的切片总是产生一个最低可能维度 11 的数组。例如，如果C是一个三维数组， C[...,1] 产生一个二维的数组而 C[1,:,1] 产生一个一维数组。从这时开始，如果相应的矩阵切片结果是相同的话，我们将只展示数组切片的结果。
假如我们想要一个数组的第一列和第三列，一种方法是使用列表切片：
>>> A[:,[1,3]]
array([[ 1,  3],
       [ 5,  7],
       [ 9, 11]])
稍微复杂点的方法是使用 take() 方法(method):
>>> A[:,].take([1,3],axis=1)
array([[ 1,  3],
       [ 5,  7],
       [ 9, 11]])
如果我们想跳过第一行，我们可以这样：
>>> A[1:,].take([1,3],axis=1)
array([[ 5,  7],
       [ 9, 11]])
或者我们仅仅使用 A[1:,[1,3]] 。还有一种方法是通过矩阵向量积(叉积)。
>>> A[ix_((1,2),(1,3))]
array([[ 5,  7],
       [ 9, 11]])
为了读者的方便，在次写下之前的矩阵：
>>> A[ix_((1,2),(1,3))]
array([[ 5,  7],
       [ 9, 11]])
现在让我们做些更复杂的。比如说我们想要保留第一行大于1的列。一种方法是创建布尔索引：
>>> A[0,:]>1
array([False, False, True, True], dtype=bool)
>>> A[:,A[0,:]>1]
array([[ 2,  3],
       [ 6,  7],
       [10, 11]])
就是我们想要的！但是索引矩阵没这么方便。
>>> M[0,:]>1
matrix([[False, False, True, True]], dtype=bool)
>>> M[:,M[0,:]>1]
matrix([[2, 3]])
这个过程的问题是用“矩阵切片”来切片产生一个矩阵 12 ，但是矩阵有个方便的 A 属性，它的值是数组呈现的。所以我们仅仅做以下替代：
>>> M[:,M.A[0,:]>1]
matrix([[ 2,  3],
        [ 6,  7],
        [10, 11]])
如果我们想要在矩阵两个方向有条件地切片，我们必须稍微调整策略，代之以：
>>> A[A[:,0]>2,A[0,:]>1]
array([ 6, 11])
>>> M[M.A[:,0]>2,M.A[0,:]>1]
matrix([[ 6, 11]])
我们需要使用向量积 ix_ :
>>> A[ix_(A[:,0]>2,A[0,:]>1)]
array([[ 6,  7],
       [10, 11]])
>>> M[ix_(M.A[:,0]>2,M.A[0,:]>1)]
matrix([[ 6,  7],
        [10, 11]])
技巧和提示
下面我们给出简短和有用的提示。
“自动”改变形状
更改数组的维度，你可以省略一个尺寸，它将被自动推导出来。
>>> a = arange(30)
>>> a.shape = 2,-1,3  # -1 means "whatever is needed"
>>> a.shape
(2, 5, 3)
>>> a
array([[[ 0,  1,  2],
        [ 3,  4,  5],
        [ 6,  7,  8],
        [ 9, 10, 11],
        [12, 13, 14]],
       [[15, 16, 17],
        [18, 19, 20],
        [21, 22, 23],
        [24, 25, 26],
        [27, 28, 29]]])

*/

'''





if __name__ == "__main__":
    matrixFunc()
