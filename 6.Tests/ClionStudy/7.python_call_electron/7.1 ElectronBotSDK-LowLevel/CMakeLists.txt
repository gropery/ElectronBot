# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.21)
# 项目名称
project(ElectronBotSDK-LowLevel)
# C++ 标准
set(CMAKE_CXX_STANDARD 14)

add_definitions("-DVSLIBDLL_EXPORTS")

# 搜索OpenCV目录
find_package(OpenCV REQUIRED)

# 包含头文件
include_directories(
        src
        lib
)

# 包含目标文件
file(GLOB_RECURSE SOURCES
        "src/*.*"
        )


link_directories(
        lib
)

# 生成链接库
add_library(${PROJECT_NAME} SHARED
        "src/electron_low_level.cpp"
        )

# 生成指定目标
add_executable(Sample
        "src/sample.cpp"
        )

# 添加链接库
target_link_libraries(${PROJECT_NAME}
        USBInterface
        )

# 添加链接库
target_link_libraries(Sample
        ${PROJECT_NAME}
        ${OpenCV_LIBS}
        )