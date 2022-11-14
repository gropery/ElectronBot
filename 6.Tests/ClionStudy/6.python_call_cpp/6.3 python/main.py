# -*- coding: utf-8 -*-
import ctypes

# 指定动态链接库
lib = ctypes.cdll.LoadLibrary('./clion_lib_shared.dll')
# 需要指定返回值的类型，默认是int
lib.my_calculate.restype = ctypes.c_double


class OBJ(ctypes.Structure):
    _fields_ = []


lib.test_new.restype = ctypes.POINTER(OBJ)


class Test(object):
    def __init__(self):
        # 动态链接对象
        self.obj = lib.test_new()

    def calculate(self, a, b, c, d, e, f):
        res = lib.my_calculate(self.obj, a, b, c, d, e, f)
        return res

    def getid(self):
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


if __name__ == '__main__':
    t = Test()
    A1 = 123
    A2 = 0.789
    A3 = "C789"
    A4 = [456, 789]
    A5 = [0.123, 0.456]
    A6 = ["A123", "B456"]
    print(t.calculate(convert_type(A1), convert_type(A2), convert_type(A3), convert_type(A4), convert_type(A5),
                      convert_type(A6)))
    print("-------------------")
    print(t.getid())
