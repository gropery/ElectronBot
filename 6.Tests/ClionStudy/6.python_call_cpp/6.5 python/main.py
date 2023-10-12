from ctypes import *
import numpy as np
import numpy.ctypeslib as npct

lib = CDLL('./clion_lib_shared.dll')
#####################################################
# 定义类数据类型
class OBJ(Structure):
    _fields_ = []

# 定义三维数据数据类型
ARRAY = npct.ndpointer(dtype=np.uint8,shape=(3,3,3))

# 指定输入参数类型和函数返回类型
lib.CreateObj.argtypes = c_int,
lib.CreateObj.restype = POINTER(OBJ)
lib.DoSomething.argtypes = POINTER(OBJ),ARRAY
lib.DoSomething.restype = c_int

####################################################
arr = np.arange(27,dtype=np.uint8).reshape((3,3,3))
# 调用dll库导出函数返回类指针
obj = lib.CreateObj(5)
# 调用dll库导出函数输入三维数据, dll库将接收到的三维数据元素都*2
# 此时python中变量arr则会是*2之后的数据
result = lib.DoSomething(obj,arr)
# 打印比较python原数据和输入dll的数据是否正确
print(f'result = {result}')
print(arr)
