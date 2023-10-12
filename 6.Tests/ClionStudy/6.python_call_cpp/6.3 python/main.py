# -*- coding: utf-8 -*-
import ctypes
# 指定动态链接库
lib = ctypes.cdll.LoadLibrary('./clion_lib_shared.dll')

#########################################################
# 指定my_calculate函数返回值的类型为c_double
lib.my_calculate.restype = ctypes.c_double

# 指定test_new函数返回值的类型为自定义类
class OBJ(ctypes.Structure):
    _fields_ = []
lib.test_new.restype = ctypes.POINTER(OBJ)

#########################################################
# python 测试类
class Test(object):
    def __init__(self):
        # 调用动态链接库函数
        self.obj = lib.test_new()

    def calculate(self, a, b, c, d, e, f):
        # 调用动态链接库函数
        res = lib.my_calculate(self.obj, a, b, c, d, e, f)
        return res

    def getid(self):
        # 调用动态链接库函数
        res = lib.my_getid(self.obj)
        return res

# 将python类型转换成c类型，支持int, float,string的变量和数组的转换
def convert_type(input):
    ctypes_map = {int: ctypes.c_int,
                  float: ctypes.c_double,
                  str: ctypes.c_char_p
                  }
    input_type = type(input)
    if input_type is list:
        length = len(input)
        if length == 0:
            print("convert type failed...input is " + input)
            return None
        else:
            arr = (ctypes_map[type(input[0])] * length)()
            for i in range(length):
                arr[i] = bytes(input[i], encoding="utf-8") if (type(input[0]) is str) else input[i]
            return arr
    else:
        if input_type in ctypes_map:
            return ctypes_map[input_type](bytes(input, encoding="utf-8") if type(input) is str else input)
        else:
            print("convert type failed...input is " + input)
            return None

#######################################################
if __name__ == '__main__':
    # 先创建python测试类Test,然后函数__init__调用dll库函数test_new(),返回dll中Test类指针保存于Test类中
    t = Test()
    A1 = 123
    A2 = 0.789
    A3 = "C789"
    A4 = [456, 789]
    A5 = [0.123, 0.456]
    A6 = ["A123", "B456"]
    print("-------------------")
    # 先调用python测试类Test函数calculate调用dll库函数my_calculate(),返回double类型数据
    print(t.calculate(convert_type(A1), convert_type(A2), convert_type(A3), convert_type(A4), convert_type(A5),
                      convert_type(A6)))
    print("-------------------")
    # 先调用python测试类Test函数my_getid调用dll库函数my_calculate(),返回double类型数据
    print(t.getid())
