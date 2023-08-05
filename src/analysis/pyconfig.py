import numpy as np
import scipy.optimize as opt

np.seterr(invalid='ignore')

def Line(x, a):
    return a * x

def BH(x, PV, YP):
    return PV * x + YP

def PL(x, n, K):
    return K * x ** n

def HB(x, n, K, tau0):
    return K * x ** n + tau0

def getR2(actual, predict):
    R2 = 0
    try:
        MSE = sum(np.power((actual[i] - predict[i]), 2) for i in range(0, len(actual))) / len(actual)
        R2 = 1 - MSE / np.var(actual)
    except:
        print('except')
    return R2

def getRegression(x, y, rangePL, rangeBH):
    poptBH = []
    poptPL = []
    poptHB = []
    poptL  = []
    try:
        popt0, pcov0 = opt.curve_fit(Line, x, y)
        popt1, pcov1 = opt.curve_fit(BH, x, y)
        popt2, pcov2 = opt.curve_fit(PL, x, y, bounds=([rangePL[0], rangePL[2]], [rangePL[1], rangePL[3]]))  # 优化PL拟合精度
        popt3, pcov3 = opt.curve_fit(HB, x, y, bounds=([rangeBH[0], rangeBH[2], rangeBH[4]], [rangeBH[1], rangeBH[3], rangeBH[5]]))  # 优化HB拟合精度
        for i in range(len(popt1)):
            poptBH.append(popt1[i])
        for i in range(len(popt2)):
            poptPL.append(popt2[i])
        for i in range(len(popt3)):
            poptHB.append(popt3[i])
    except:
        print('except')
    return poptBH, poptPL, poptHB, popt0