import cv2 as cv
from ctypes import *
import numpy as np
import numpy.ctypeslib as npct
import SpaceClock as sc

####################################################
# ctype 加载
lib = CDLL('./ElectronBotSDK-LowLevel.dll')


# Robot对象
class OBJ(Structure):
    _fields_ = []


ARRAY = npct.ndpointer(dtype=np.uint8, shape=(240, 240, 3))
lib.CreateElectronLowLevel.restype = POINTER(OBJ)
lib.mySetImageSrc.argtypes = POINTER(OBJ), ARRAY,
lib.mySetJointAngles.argtypes = POINTER(OBJ), POINTER(c_float), c_int
lib.myGetJointAngles.restype = POINTER(c_float)

robot = lib.CreateElectronLowLevel()
if lib.myConnect(robot):
    robotIsConnected = True
    print("Robot connected!\n")
else:
    robotIsConnected = False
    print("Connect failed!\n")


# 将python类型转换成c类型，支持int, float, string的变量和数组的转换
def convert_type(input):
    ctypes_map = {int: c_int,
                  float: c_float,
                  str: c_char_p
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


jointAngles1 = [0.0, 0.0, 0.0, 0.0, 0.0, 0.0]  # 预设定舵机角度值
jointAnglesEn = 0  # 舵机设定使能 1:使能 0:失能

####################################################
# 图像获取
# cap = cv.VideoCapture('./happy.mp4')
# cap = cv.VideoCapture(0)
# while cap.isOpened():
#     ret, frame = cap.read()
#     # 如果正确读取帧，ret为True
#     if not ret:
#         print("Can't receive frame (stream end?). Exiting ...")
#         break

# 加载太空人表盘
s = sc.spaceclock()  # 太空人对象
s.loadImgs()  # 太空人图片素材加载
img = np.zeros((s.WIN_SIZE, s.WIN_SIZE, 3), np.uint8)
index = 0

while True:
    img = s.gameDraw(img)  # 太空人表盘内容绘制
    s.animation(img, index)  # 太空人旋转绘制
    index = (index + 1) % 59
    frame = img

    cv.imshow('image', frame)
    if cv.waitKey(1) == ord('q'):
        break

    if robotIsConnected:
        # height, weight, channel = img.shape
        # blue, green, red = channel

        frame = cv.resize(frame, (240, 240))
        frame = cv.cvtColor(frame, cv.COLOR_BGRA2RGB)

        # 向robot dll缓存写入图像和舵机数据
        lib.mySetImageSrc(robot, frame)
        lib.mySetJointAngles(robot, convert_type(jointAngles1), convert_type(jointAnglesEn))
        # 调用robot写入图像和舵机数据至device, 并读回当前舵机位置值
        lib.mySync(robot)
        # 获取当前舵机位置值
        jointAngles2 = lib.myGetJointAngles(robot)
        # print(jointAngles2[0])
        # print(jointAngles2[5])

if robotIsConnected:
    lib.myDisconnect(robot)
    print("File play finished, robot Disconnected!\n")

cv.destroyAllWindows()
