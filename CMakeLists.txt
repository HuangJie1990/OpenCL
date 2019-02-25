cmake_minimum_required(VERSION 3.10)
project(CL)

set(CMAKE_CXX_STANDARD 11)

#linux(ubuntu 14.04) with OpenCL™ Runtimes for Intel® Processors installed
#using defualt toolchain

include_directories(/opt/intel/opencl-sdk/include)
link_directories(/opt/intel/opencl-sdk/lib64)

#host linux with OpenCL™ Runtimes for Intel® Processors installed