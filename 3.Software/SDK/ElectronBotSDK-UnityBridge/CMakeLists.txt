# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.20)
# 项目名称
project(ElectronBotSDK-UnityBridge)
# C++ 标准
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_C_STANDARD 11)
# 解决windows系统下Clion搭配VS编译器报字符警告
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

######################################################################
set(CMAKE_PREFIX_PATH   D:\\Libraries\\Cpp\\opencv4.6.0\\build)
# 搜索OpenCV目录
find_package(OpenCV REQUIRED)
# 包含头文件
include_directories(
        src
        lib
)
# 指定要链接的库文件路径
link_directories(
        lib
)
######################################################################
# 添加链接库
add_library(${PROJECT_NAME} SHARED
        src/electron_sdk_unity_bridge.cpp)

######################################################################
# ElectronBotSDK-UnityBridge 动态库 链接 ElectronBotSDK-LowLevel和opencv库
target_link_libraries(${PROJECT_NAME}
        ElectronBotSDK-LowLevel;
        ${OpenCV_LIBS}
        )
