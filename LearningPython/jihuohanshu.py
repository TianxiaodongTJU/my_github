#画出阶跃函数和sigmod函数的图形
import numpy as np
import matplotlib.pylab as plt

#阶跃函数
def step_function(x):
    return np.array(x>0, dtype=np.int)

x = np.arange(-5.0,5.0,0.1)
y = step_function(x)

plt.plot(x,y)
plt.ylim(-0.1,1.1)
plt.show

#sigmoid函数
def sigmoid(x):
    return 1 / (1 + np.exp(-x))

y = sigmoid(x)
plt.plot(x,y)
plt.ylim(-0.1,1.1)
plt.show


#输出函数分为两种，恒等函数和softmax函数
#恒等函数常用于回归问题
def identity_function(x):
    return x


#softmax函数常用于分类问题
def softmax(a):
    c = np.max(a)
    exp_a = np.exp(a - c) #防止溢出
    sum_exp_a = np.sum(exp_a)
    y = exp_a / sum_exp_a
    
    return y

    
    


