# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)

project(OSQPWrapper-Example)

find_package(OSQPWrapper)

include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

#MPCExample
add_executable(MPCExample src/MPCExample.cpp)
target_link_libraries(MPCExample OSQPWrapper::OSQPWrapper osqp::osqp)
