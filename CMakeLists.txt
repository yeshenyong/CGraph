
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")
message("*     ______   ______                             __    *")
message("*    / ____/  / ____/   _____  ____ _    ____    / /_   *")
message("*   / /      / / __    / ___/ / __ `/   / __ \\  / __ \\  *")
message("*  / /___   / /_/ /   / /    / /_/ /   / /_/ / / / / /  *")
message("*  \\____/   \\____/   /_/     \\__,_/   / .___/ /_/ /_/   *")
message("*                                    /_/ by Chunel Feng *")
message("* * * * * * * * * * * * * * * * * * * * * * * * * * * * *")

cmake_minimum_required(VERSION 3.5.0)

project(CGraph VERSION 2.2.0)

# CGraph推荐使用C++17或以上版本。支持无缝切换C++14版本，仅有个别功能功能的弱化和剪裁。暂不支持C++11或以下版本
set(CMAKE_CXX_STANDARD 17)

# add CGraph environment info
include(cmake/CGraph-env-include.cmake)

file(GLOB_RECURSE CGRAPH_SRC_LIST "./src/*.cpp")

# 如果开启此宏定义，则CGraph执行过程中，不会在控制台打印任何信息
# add_definitions(-D_CGRAPH_SILENCE_)

# 编译libCGraph动态库
# add_library(CGraph SHARED ${CGRAPH_SRC_LIST})

set(CGRAPH_TUTORIAL_LIST
        T05-Param


        # 以下为工具类tutorial

        )

foreach(tut ${CGRAPH_TUTORIAL_LIST})
    add_executable(${tut} tutorial/${tut}.cpp ${CGRAPH_SRC_LIST})
endforeach()
