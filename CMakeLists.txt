cmake_minimum_required(VERSION 3.23)
project(demo)

set(CMAKE_CXX_STANDARD 17)

add_executable(demo main.cpp
        core/ISystemController.h
        core/IPmuCollector.h
        workloads/workloads.cpp
        core/PmuCollector.cpp
        core/PmuCollector.h
        core/SystemController.cpp
        core/SystemController.h
        workloads/workloads.h)
