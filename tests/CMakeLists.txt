# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)

project(OSQP-Wrapper-Test)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# SparseMatrixTest
add_executable(SparseMatrixTest SparseMatrixTest.cpp)
target_link_libraries(SparseMatrixTest OSQPWrapper ${GTEST_LIBRARIES} pthread)
add_test(NAME SparseMatrixTest
  COMMAND SparseMatrixTest)
