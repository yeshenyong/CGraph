message("By YeShenYong")
message("*************")


cmake_minimum_required(VERSION 3.1)

project(cgraph VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O2 -pthread")

file(GLOB_RECURSE CGRAPH_SRC_LIST "../src/*.cpp")

find_package(pybind11 REQUIRED)

pybind_add_module(cgraph ./c/py_export.cpp ${CGRAPH_SRC_LIST})

