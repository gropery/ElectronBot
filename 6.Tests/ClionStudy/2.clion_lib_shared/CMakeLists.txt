cmake_minimum_required(VERSION 3.23)
project(clion_lib_shared)

set(CMAKE_CXX_STANDARD 14)

add_definitions("-DVSLIBDLL_EXPORTS")

add_library(clion_lib_shared SHARED module/fun.cpp)
