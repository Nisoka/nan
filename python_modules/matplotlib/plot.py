#!/usr/bin/env python
# -*- coding: utf-8 -*-


# matplotlib
# http://blog.csdn.net/u011497262/article/details/52325705

import numpy as np
import matplotlib.pyplot as plt

def simpleSin():
    #numpy basefunc linspace(start, end, cnt, ifContainEndpoint)
    #   ufunc np.cos() 直接对ndarray 进行通用计算
    X = np.linspace(-np.pi, np.pi, 256, endpoint = True)
    C, S = np.cos(X), np.sin(X)
    #plt func plot(x, y)
    plt.plot(X, C)
    plt.plot(X, S)
    plt.show()


# 导入 matplotlib 的所有内容（nympy 可以用 np 这个名字来使用）
#from pylab import *
def funcDefaultSet():

    '''
    1 创建图figure ,
           是否一定需要figure 调用？可能不需要会有一个默人的figure

     创建一个 8 * 6 点（point）的图，并设置分辨率为 80
     8 * 6 代表 图的X Y 方向上图像大小.(8*80 X 6*80)
    '''
    plt.figure(figsize=(8,6), dpi=80)
    '''
    2 创建一个新的 1 * 1 的子图，接下来的图样绘制在其中的第 1 块（也是唯一的一块）
    '''
    plt.subplot(1,1,1)


    X = np.linspace(-np.pi, np.pi, 256,endpoint=True)
    C,S = np.cos(X), np.sin(X)

#    # 绘制余弦曲线，使用蓝色的、连续的、宽度为 1 （像素）的线条
#    plt.plot(X, C, color="blue", linewidth=1.0, linestyle="-")
#    # 绘制正弦曲线，使用绿色的、连续的、宽度为 1 （像素）的线条
#    plt.plot(X, S, color="green", linewidth=1.0, linestyle="-")
    '''
    3 绘制曲线
    曲线的绘制 是 用点连接起来形成的线。
        plot(x, y [, settings])
    '''
    plt.plot(X, C, color="black", linewidth=3.0, linestyle="-")
    plt.plot(X, S, color="red", linewidth=3.0, linestyle="-")

    '''
    4 坐标轴
    上下限
        xlim(start, end)
        xlim(list)

    刻度标记
        xticks(ndarray)
            必须要和start end 匹配
        好像xticks 内部还会调用xlim
        eg
            如果 xlim(-4, 4)
            如果再调用 xticks(-100, 100) 那么X轴 的上下限 直接就变为了 -100, 100

        ? 上下限 是否可以和标记不匹配？  XXXXX
            例如设置好上线先 -5 -- +5 但是希望显示为 任意符号[a b c d e f g]?
        plt.xticks(['a', 'b', 'c', 'd', 'e'])
        不可以

    '''
    # 设置横轴的上下限
    plt.xlim(-4.0,4.0)
    # 设置横轴记号
    plt.xticks(np.linspace(-4,4,9,endpoint=True))

    plt.ylim(-1.0,1.0)
    plt.yticks(np.linspace(-1,1,5,endpoint=True))

    # 美化 曲线显示情况
    # 1 曲线显示区域 居中美化
    plt.xlim(X.min()*1.1, X.max()*1.1)
    plt.ylim(C.min()*1.1, C.max()*1.1)
    #
    xmin ,xmax = X.min(), X.max()
    ymin, ymax = C.min(), C.max()
    dx = (xmax - xmin) * 0.2
    dy = (ymax - ymin) * 0.2
    plt.xlim(xmin - dx, xmax + dx)
    plt.ylim(ymin - dy, ymax + dy)

    # 2 改变显示ticks刻度信息
    plt.xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi])
    plt.yticks([-1, 0, +1])
    # 让刻度信息 更加美化？？使用了 LaTeX(数学公式 输入)
    plt.xticks([-np.pi, -np.pi/2, 0, np.pi/2, np.pi],
           [r'$-\pi$', r'$-\pi/2$', r'$0$', r'$+\pi/2$', r'$+\pi$'])

    plt.yticks([-1, 0, +1],
           [r'$-1$', r'$0$', r'$+1$'])



    '''
    5 移动脊柱
        移动坐标轴线 刻度线
        坐标轴线和上面的记号连在一起就形成了脊柱（Spines，一条线段上有一系列的凸起，
    是不是很像脊柱骨啊~），它记录了数据区域的范围。它们可以放在任意位置，不过至今为止，
    我们都把它放在图的四边。
        四条坐标轴线  组成字典dict (key-value) {"left": leftline, "right": rightline ..}
        实际上每幅图有四条脊柱（上下左右），为了将脊柱放在图的中间，我们必须
    将其中的两条（上和右）设置为无色，然后调整剩下的两条到合适的位置——数据空间的 0 点。
    '''
    ax = plt.gca()
    ax.spines['right'].set_color('none')
    ax.spines['top'].set_color('none')

    ax.xaxis.set_ticks_position('bottom')
    ax.spines['bottom'].set_position(('data',0))
    ax.yaxis.set_ticks_position('left')
    ax.spines['left'].set_position(('data',0))

    '''
    6 添加图例
        图示 什么曲线代表什么的信息
        我们在图的左上角添加一个图例。
        为此，我们只需要在 plot 函数里以「键 - 值」的形式增加一个参数。
        legend 图例显示
    '''
    plt.plot(X, C, color="blue", linewidth=2.5, linestyle="-", label="cosine")
    plt.plot(X, S, color="red",  linewidth=2.5, linestyle="-", label="sine")

    plt.legend(loc='upper left')


    '''
    7 给一些特殊点做注释
        我们希望在 2π/3 2π/3 的位置给两条函数曲线加上一个注释。
    首先，我们在对应的函数图像位置上画一个点；
    然后，向横轴引一条垂线，以虚线标记；
    最后，写上标签。

    '''
    t = 2*np.pi/3
    plt.plot([t, t], [0, np.cos(t)], color ='blue', linewidth=2.5, linestyle="--")
    # scatter 绘制点 plot 绘制线
    plt.scatter([t, ], [np.cos(t), ], 50, color ='blue')

    plt.annotate(r'$\sin(\frac{2\pi}{3})=\frac{\sqrt{3}}{2}$',
             xy = (t, np.sin(t)), xycoords = 'data',
             xytext = (+10, +30), textcoords = 'offset points', fontsize = 16,
             arrowprops = dict(arrowstyle = "->", connectionstyle = "arc3,rad=.2"))

    plt.plot([t,t],[0,np.sin(t)], color ='red', linewidth=2.5, linestyle="--")
    plt.scatter([t,],[np.sin(t),], 50, color ='red')
    plt.annotate(r'$\cos(\frac{2\pi}{3})=-\frac{1}{2}$',
             xy=(t, np.cos(t)), xycoords='data',
             xytext=(-90, -50), textcoords='offset points', fontsize=16,
             arrowprops=dict(arrowstyle="->", connectionstyle="arc3,rad=.2"))


    '''
    8 精益求精  坐标轴标签遮挡问题解决
        坐标轴上的记号标签被曲线挡住了，作为强迫症患者（雾）这是不能忍的。
        我们可以把它们放大，然后添加一个白色的半透明底色。这样可以保证标签和曲线同时可见。
    '''
    for label in ax.get_xticklabels() + ax.get_yticklabels():
        label.set_fontsize(16)
        label.set_bbox(dict(facecolor='white', edgecolor='None', alpha=0.65 ))

    # 以分辨率 72 来保存图片
    # savefig("exercice_2.png",dpi=72)

    # 在屏幕上显示
    plt.show()




'''
图像、子图、坐标轴和记号
    到目前为止，我们都用隐式的方法来绘制图像和坐标轴。快速绘图中，这是很方便的。
    我们也可以显式地控制图像、子图、坐标轴。

    Matplotlib 中的「图像」指的是用户界面看到的整个窗口内容。
    在图像里面有所谓「子图」。子图的位置是由坐标网格确定的.
    而「坐标轴」却不受此限制，可以放在图像的任意位置。

    我们已经隐式地使用过图像和子图：
        当我们调用 plot 函数的时候，matplotlib 调用
            gca() 获取当前的坐标轴
            gcf() 获取当前的图像
        如果无法获取图像，则会调用 figure() 函数来创建一个图像.
        严格地说，是用 subplot(1,1,1) 创建一个只有一个子图的图像。

图像 -- 就是一个窗口
    所谓「图像」就是 GUI 里以「Figure #」为标题的那些窗口。
    图像编号从 1 开始，与 MATLAB 的风格一致，而于 Python 从 0 开始编号的风格不同。

图像的属性：
参数          默认值         描述
num             1           图像的数量
figsize     figure.figsize	图像的长和宽（英寸）
dpi         figure.dpi      分辨率（点/英寸）
facecolor	figure.facecolor	绘图区域的背景颜色
edgecolor	figure.edgecolor	绘图区域边缘的颜色
frameon     True            是否绘制图像边缘

这些默认值可以在源文件中指明。
和其他对象一样, 你可以使用 setp 或者是 set_something 这样的方法来设置图像的属性。
不过除了图像数量这个参数，其余的参数都很少修改。



close
    你在图形界面中可以按下右上角的 X 来关闭窗口（OS X 系统是左上角）。
    Matplotlib 也提供了名为 close 的函数来关闭这个窗口。
    close 函数的具体行为取决于你提供的参数：
    不传递参数：关闭当前窗口；
    传递窗口编号 或 窗口实例（instance）作为参数：关闭指定的窗口；
    all：关闭所有窗口。



子图
    你可以用子图来将图样（plot）放在均匀的坐标网格中。
    用 subplot 函数的时候，你需要指明网格的 行 列 数量，以及你希望将图样放在哪一个网格区域中。
    此外，gridspec 的功能更强大，你也可以选择它来实现这个功能。

坐标轴
    坐标轴和子图功能类似，不过它可以放在图像的任意位置。
    因此，如果你希望在一副图中绘制一个小图，就可以用这个功能。

记号
    良好的记号是图像的重要组成部分。
    Matplotlib 里的记号系统里的各个细节都是可以由用户个性化配置的。
    你可以
        用 Tick Locators 来指定在那些位置放置记号，
        用 Tick Formatters 来调整记号的样式。
        主要和次要的记号可以以不同的方式呈现。默认情况下，每一个次要的记号都是隐藏的，
        也就是说，默认情况下的次要记号列表是空的——NullLocator。

    Tick Locators
    下面有为不同需求设计的一些 Locators。
    ...
    这些 Locators 都是 matplotlib.ticker.Locator 的子类，你可以据此定义自己的 Locator。
    以日期为 ticks 特别复杂，因此 Matplotlib 提供了 matplotlib.dates 来实现这一功能。

'''



'''
其他类型的图
    接下来的内容是练习。
    请运用你学到的知识，从提供的代码开始，实现配图所示的效果。具体的答案可以点击配图下载。
'''
def funcBrushSin():
    n = 256
    X = np.linspace(-np.pi,np.pi,n,endpoint=True)
    Y = np.sin(2*X)

    plt.axes([0.025,0.025,0.95,0.95])

    plt.plot (X, Y+1, color='blue', alpha=1.00)
    plt.fill_between(X, 1, Y+1, color='blue', alpha=.25)

    plt.plot (X, Y-1, color='blue', alpha=1.00)
    plt.fill_between(X, -1, Y-1, (Y-1) > -1, color='blue', alpha=.25)
    plt.fill_between(X, -1, Y-1, (Y-1) < -1, color='red',  alpha=.25)

    plt.xlim(-np.pi,np.pi), plt.xticks([])
    plt.ylim(-2.5,2.5), plt.yticks([])
    # savefig('../figures/plot_ex.png',dpi=48)
    plt.show()


def funcScatter():
    n = 1024
    X = np.random.normal(0,1,n)
    Y = np.random.normal(0,1,n)
    T = np.arctan2(Y,X)

    plt.axes([0.025,0.025,0.95,0.95])
    plt.scatter(X,Y, s=75, c=T, alpha=.5)

    plt.xlim(-1.5,1.5), plt.xticks([])
    plt.ylim(-1.5,1.5), plt.yticks([])
    # savefig('../figures/scatter_ex.png',dpi=48)
    plt.show()

def funcBars():
    n = 12
    X = np.arange(n)
    Y1 = (1-X/float(n)) * np.random.uniform(0.5,1.0,n)
    Y2 = (1-X/float(n)) * np.random.uniform(0.5,1.0,n)

    plt.axes([0.025,0.025,0.95,0.95])
    plt.bar(X, +Y1, facecolor='#9999ff', edgecolor='white')
    plt.bar(X, -Y2, facecolor='#ff9999', edgecolor='white')

    for x,y in zip(X,Y1):
        plt.text(x+0.4, y+0.05, '%.2f' % y, ha='center', va= 'bottom')

    for x,y in zip(X,Y2):
        plt.text(x+0.4, -y-0.05, '%.2f' % y, ha='center', va= 'top')

    plt.xlim(-.5,n), plt.xticks([])
    plt.ylim(-1.25,+1.25), plt.yticks([])

    # savefig('../figures/bar_ex.png', dpi=48)
    plt.show()

def funcLevelHigh():
    def f(x,y):
        return (1-x/2+x**5+y**3)*np.exp(-x**2-y**2)

    n = 256
    x = np.linspace(-3,3,n)
    y = np.linspace(-3,3,n)
    X,Y = np.meshgrid(x,y)

    plt.axes([0.025,0.025,0.95,0.95])

    plt.contourf(X, Y, f(X,Y), 8, alpha=.75, cmap=plt.cm.hot)
    C = plt.contour(X, Y, f(X,Y), 8, colors='black', linewidth=.5)
    plt.clabel(C, inline=1, fontsize=10)

    plt.xticks([]), plt.yticks([])
    # savefig('../figures/contour_ex.png',dpi=48)
    plt.show()


def f(x,y):
    return (1-x/2+x**5+y**3)*np.exp(-x**2-y**2)

def funcGray():

    n = 10
    x = np.linspace(-3,3,3.5*n)
    y = np.linspace(-3,3,3.0*n)
    X,Y = np.meshgrid(x,y)
    Z = f(X,Y)

    plt.axes([0.025,0.025,0.95,0.95])
    plt.imshow(Z,interpolation='nearest', cmap='bone', origin='lower')
    plt.colorbar(shrink=.92)

    plt.xticks([]), plt.yticks([])
    # savefig('../figures/imshow_ex.png', dpi=48)
    plt.show()


def funcPie():
    n = 20
    Z = np.ones(n)
    Z[-1] *= 2

    plt.axes([0.025,0.025,0.95,0.95])

    plt.pie(Z, explode=Z*.05, colors = ['%f' % (i/float(n)) for i in range(n)])
    plt.gca().set_aspect('equal')
    plt.xticks([]), plt.yticks([])

    # savefig('../figures/pie_ex.png',dpi=48)
    plt.show()

# 量场图
def funcQuiver():
    n = 8
    X,Y = np.mgrid[0:n,0:n]
    T = np.arctan2(Y-n/2.0, X-n/2.0)
    R = 10+np.sqrt((Y-n/2.0)**2+(X-n/2.0)**2)
    U,V = R*np.cos(T), R*np.sin(T)

    plt.axes([0.025,0.025,0.95,0.95])
    plt.quiver(X,Y,U,V,R, alpha=.5)
    plt.quiver(X,Y,U,V, edgecolor='k', facecolor='None', linewidth=.5)

    plt.xlim(-1,n), plt.xticks([])
    plt.ylim(-1,n), plt.yticks([])

    # savefig('../figures/quiver_ex.png',dpi=48)
    plt.show()




# 绘制坐标网格
def funcAxasGrid():
    ax = plt.axes([0.025,0.025,0.95,0.95])

    ax.set_xlim(0,4)
    ax.set_ylim(0,3)
    ax.xaxis.set_major_locator(plt.MultipleLocator(1.0))
    ax.xaxis.set_minor_locator(plt.MultipleLocator(0.1))
    ax.yaxis.set_major_locator(plt.MultipleLocator(1.0))
    ax.yaxis.set_minor_locator(plt.MultipleLocator(0.1))
    ax.grid(which='major', axis='x', linewidth=0.75, linestyle='-', color='0.75')
    ax.grid(which='minor', axis='x', linewidth=0.25, linestyle='-', color='0.75')
    ax.grid(which='major', axis='y', linewidth=0.75, linestyle='-', color='0.75')
    ax.grid(which='minor', axis='y', linewidth=0.25, linestyle='-', color='0.75')
    ax.set_xticklabels([])
    ax.set_yticklabels([])

    # savefig('../figures/grid_ex.png',dpi=48)
    plt.show()


# 多子图 绘制
def funcSubPlot():
    fig = plt.figure()
    fig.subplots_adjust(bottom=0.025, left=0.025, top = 0.975, right=0.975)

    plt.subplot(2,1,1)
    plt.xticks([]), plt.yticks([])

    plt.subplot(2,3,4)
    plt.xticks([]), plt.yticks([])

    plt.subplot(2,3,5)
    plt.xticks([]), plt.yticks([])

    plt.subplot(2,3,6)
    plt.xticks([]), plt.yticks([])

    # plt.savefig('../figures/multiplot_ex.png',dpi=48)
    plt.show()


# 极轴图
def funcBarZeroVec():
    ax = plt.axes([0.025,0.025,0.95,0.95], polar=True)

    N = 20
    theta = np.arange(0.0, 2*np.pi, 2*np.pi/N)
    radii = 10*np.random.rand(N)
    width = np.pi/4*np.random.rand(N)
    bars = plt.bar(theta, radii, width=width, bottom=0.0)

    for r,bar in zip(radii, bars):
        bar.set_facecolor( plt.cm.jet(r/10.))
        bar.set_alpha(0.5)

    ax.set_xticklabels([])
    ax.set_yticklabels([])
    # savefig('../figures/polar_ex.png',dpi=48)
    plt.show()



def func3DPlot():
    from mpl_toolkits.mplot3d import Axes3D

    fig = plt.figure()
    ax = Axes3D(fig)
    X = np.arange(-4, 4, 0.25)
    Y = np.arange(-4, 4, 0.25)
    X, Y = np.meshgrid(X, Y)
    R = np.sqrt(X**2 + Y**2)
    Z = np.sin(R)

    ax.plot_surface(X, Y, Z, rstride=1, cstride=1, cmap=plt.cm.hot)
    ax.contourf(X, Y, Z, zdir='z', offset=-2, cmap=plt.cm.hot)
    ax.set_zlim(-2,2)

    # savefig('../figures/plot3d_ex.png',dpi=48)
    plt.show()



'''

'''

'''
结束篇
    Matplotlib 能有今天这样强大的功能和广泛的使用得益于大量的文档和社区开发者。
    这里提供一些有益的链接。

入门教程
    Pyplot tutorial
    Image tutorial
    Text tutorial
    Artist tutorial
    Path tutorial
    Transforms tutorial

Matplotlib 文档
    随代码分发的文档
    Matplotlib 的代码是自文档（Self-documented）的。你可以在 Python 中快速查询某个命令的用法。

>>> from pylab import *
>>> help(plot)
Help on function plot in module matplotlib.pyplot:

plot(*args, **kwargs)
   Plot lines and/or markers to the
   :class:`~matplotlib.axes.Axes`.  *args* is a variable length
   argument, allowing for multiple *x*, *y* pairs with an
   optional format string.  For example, each of the following is
   legal::

       plot(x, y)         # plot x and y using default line style and color
       plot(x, y, 'bo')   # plot x and y using blue circle markers
       plot(y)            # plot y using x as index array 0..N-1
       plot(y, 'r+')      # ditto, but with red plusses

   If *x* and/or *y* is 2-dimensional, then the corresponding columns
   will be plotted.
   ...


画廊
    Matplotlib 画廊 也非常有用。
    其中的例子都有配图和对应的代码，当你不知道某一个效果如何实现的时候，你可以在这里找找。
    哦，这里还有一个小一点的画廊。
'''


if __name__ == "__main__":
    funcScatter()
