# CMake 最低版本号要求
cmake_minimum_required(VERSION 3.21)
# 项目名称
project(ElectronBotSDK-LowLevel)
# C++ 标准
set(CMAKE_CXX_STANDARD 14)

# 解决windows系统下Clion搭配VS编译器报字符警告
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_PREFIX_PATH   D:\\Libraries\\Cpp\\opencv4.6.0\\build)

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

# 指定要链接的库文件路径
link_directories(
        lib
)

# 添加链接库
add_library(${PROJECT_NAME} SHARED
        "src/electron_low_level.cpp"
        )

# 生成指定目标
add_executable(Sample
        "src/sample.cpp"
        )

# ElectronBotSDK-LowLevel 动态库 链接 USB和opencv库
target_link_libraries(${PROJECT_NAME}
        USBInterface
        ${OpenCV_LIBS}
        )

# 应用sample 链接  ElectronBotSDK-LowLevel 动态库
target_link_libraries(Sample
        ${PROJECT_NAME}
        )