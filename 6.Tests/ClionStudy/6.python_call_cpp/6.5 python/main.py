from ctypes import *
import numpy as np
import numpy.ctypeslib as npct

lib = CDLL('./clion_lib_shared.dll')

class OBJ(Structure):
    _fields_ = []

ARRAY = npct.ndpointer(dtype=np.uint8,shape=(3,3,3))

lib.CreateObj.argtypes = c_int,
lib.CreateObj.restype = POINTER(OBJ)
lib.DoSomething.argtypes = POINTER(OBJ),ARRAY
lib.DoSomething.restype = c_int

arr = np.arange(27,dtype=np.uint8).reshape((3,3,3))
obj = lib.CreateObj(5)
result = lib.DoSomething(obj,arr)
print(f'result = {result}')
print(arr)