cmake_minimum_required(VERSION 3.10)
project(CL)

set(CMAKE_CXX_STANDARD 11)

#linux(ubuntu 14.04) with OpenCL™ Runtimes for Intel® Processors installed
#using defualt toolchain

include_directories(/usr/local/cuda/include)
link_directories(/usr/local/cuda/lib64)

#host linux with OpenCL™ Runtimes for Intel® Processors installed
add_executable(buffer_check buffer_check.c)

target_link_libraries(buffer_check OpenCL)
add_executable(simpleMultiply simpleMultiply.cpp)