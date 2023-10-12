cmake_minimum_required(VERSION 3.23)
project(clion_opencv)

set(CMAKE_CXX_STANDARD 14)

#set(CMAKE_PREFIX_PATH   D:\\Libraries\\Cpp\\opencv4.6.0\\build)

# 搜索OpenCV目录
find_package(OpenCV REQUIRED)

# 信息输出(非必须)
message(STATUS "OpenCV library status:")
message(STATUS "    config: ${OpenCV_DIR}")
message(STATUS "    version: ${OpenCV_VERSION}")
message(STATUS "    libraries: ${OpenCV_LIBS}")
message(STATUS "    include path: ${OpenCV_INCLUDE_DIRS}")

add_executable(clion_opencv main.cpp)

# 链接OpenCV库文件
target_link_libraries(clion_opencv ${OpenCV_LIBS})
