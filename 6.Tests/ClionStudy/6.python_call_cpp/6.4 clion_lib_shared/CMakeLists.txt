cmake_minimum_required(VERSION 3.23)
project(clion_lib_shared)

set(CMAKE_CXX_STANDARD 14)

# 解决windows系统下Clion搭配VS编译器报字符警告
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

add_definitions("-DVSLIBDLL_EXPORTS")

add_library(clion_lib_shared SHARED module/fun.cpp)
