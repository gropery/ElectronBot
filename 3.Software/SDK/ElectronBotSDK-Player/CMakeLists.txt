cmake_minimum_required(VERSION 3.21)
project(ElectronBotSDK-Player)
set(CMAKE_CXX_STANDARD 14)

# 解决windows系统下Clion搭配VS编译器报字符警告
add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

set(CMAKE_PREFIX_PATH   D:\\Libraries\\Cpp\\opencv4.6.0\\build)

# 搜索OpenCV目录
find_package(OpenCV REQUIRED)

include_directories(
        src
        lib
)

file(GLOB_RECURSE SOURCES
        "src/*.*"
        )

link_directories(
        lib
)

add_library(${PROJECT_NAME} SHARED
        "src/electron_player.cpp"
        )

add_executable(Sample
        "src/sample.cpp"
        )

target_link_libraries(${PROJECT_NAME}
        ElectronBotSDK-LowLevel
        ${OpenCV_LIBS}
        )

target_link_libraries(Sample
        ${PROJECT_NAME}
        )