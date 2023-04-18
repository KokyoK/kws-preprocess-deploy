import ctypes
import numpy as np

# 加载动态链接库
lib = ctypes.CDLL('./e.so')

# 指定函数参数类型
lib.trans_audio.argtypes = [ctypes.c_float*108480,ctypes.c_float* 4040]

# 指定函数返回值类型
lib.trans_audio.restype = ctypes.POINTER(ctypes.c_float*4040)

# 调用函数
feat = np.zeros(4040, dtype=np.float32)
wavform = np.zeros(108480, dtype=np.float32)
# 将numpy数组转换为C语言数组
feat = np.ctypeslib.as_ctypes(feat)
wavform = np.ctypeslib.as_ctypes(wavform)
result = lib.trans_audio(wavform,feat)

result_np = np.ctypeslib.as_array(result, shape=(1,))
print(result_np.shape)
print(result_np)

