import ctypes
import numpy as np

# 加载动态链接库
lib = ctypes.CDLL('./e.so')

# 指定函数参数类型
lib.cal_gcd.argtypes = [ctypes.c_float, ctypes.c_float]
lib.add.argtypes = [ctypes.c_float, ctypes.c_float]

# 指定函数返回值类型
lib.add.restype = ctypes.c_int

# 调用函数
result = lib.trans_audio(16000,np.zeros(40,101))
# result = lib.add(1,2)
print(result)  # 输出3