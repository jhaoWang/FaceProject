cmake_minimum_required(VERSION 3.18)
project(LaFinInfer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

# 包含当前目录
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

# ONNXRuntime
set(ONNXRUNTIME_DIR "D:/Installation/onnxruntime/onnxruntime")
include_directories(${ONNXRUNTIME_DIR}/include)
link_directories(${ONNXRUNTIME_DIR}/lib)

# 编译 DLL
add_library(LaFinInfer SHARED Lafin.cpp Lafin.h)
target_link_libraries(LaFinInfer ${OpenCV_LIBS} onnxruntime)

# 编译 EXE
add_executable(ExampleMain ExampleMain.cpp)
target_link_libraries(ExampleMain LaFinInfer ${OpenCV_LIBS})