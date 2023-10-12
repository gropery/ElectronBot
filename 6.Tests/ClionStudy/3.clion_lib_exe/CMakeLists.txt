##########################################################
# 设置CMake所需的最低版本
cmake_minimum_required(VERSION 3.23)
# 声明了项目的名称
project(clion_demo C CXX)
# 设置语言标准
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 14)
# 解决windows系统下Clion搭配VS编译器报字符警告
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
##########################################################
# 编译器预定义
add_definitions(-DVSLIBDLL_EXPORTS)
# 头文件目录
include_directories(
        src
        module
        lib
        )
# 目标文件目录
file(GLOB_RECURSE SOURCES
        "src/*.*"
        "module/*.*"
        )
# 指定要链接的库文件路径
link_directories(
        lib
)
##########################################################
# 创建目标——库
add_library(module SHARED
        "module/module.cpp"
        "module/module.h"
        )
# 创建目标-可执行文件
add_executable(clion_demo src/main.cpp)
##########################################################
# 连接库文件
target_link_libraries(module
        clion_lib_shared
        )
# 连接库文件
target_link_libraries(clion_demo
        module
        )
