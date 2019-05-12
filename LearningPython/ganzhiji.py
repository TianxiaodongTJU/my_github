#将theta换成-b，则w1、w2是权重，b为偏置
#感知机的计算可以转换为矩阵计算wX+b
import numpy as np

def AND(x):
    w = np.array([0.5,0.5])
    b = -0.7
    tmp = np.sum(x*w)+b
    if tmp>0:
        return 1
    else:
        return 0

print(AND(np.array([1,1])))

#感知机的局限性，单个无法实现异或
#但是可以通过使用多层感知机进行实现

def OR(x):
    w = np.array([0.5,0.5])
    b = -0.2
    tmp = np.sum(x*w)+b
    if tmp>0:
        return 1
    else:
        return 0
    
def NAND(x):
    w = np.array([-0.5,-0.5])
    b = 0.7
    tmp = np.sum(x*w)+b
    if tmp>0:
        return 1
    else:
        return 0

def XOR(x):
    s1 = NAND(x)
    s2 = OR(x)
    y  = AND(np.array([s1,s2]))
    return y

print(XOR(np.array([0,1])))
print(XOR(np.array([1,0])))
print(XOR(np.array([0,0])))
print(XOR(np.array([1,1])))