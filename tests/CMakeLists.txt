# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)

project(OSQP-Wrapper-Test)

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# QPTest
add_executable(SparseMatrixTest SparseMatrixTest.cpp)
target_link_libraries(SparseMatrixTest osqp-eigen osqp::osqp ${GTEST_LIBRARIES} pthread)
add_test(NAME SparseMatrixTest
  COMMAND SparseMatrixTest)

# QPTest
add_executable(QPTest QPTest.cpp)
target_link_libraries(QPTest osqp-eigen osqp::osqp ${GTEST_LIBRARIES} pthread)
add_test(NAME QPTest
  COMMAND QPTest)

# Update matrix
add_executable(UpdateMatricesTest UpdateMatricesTest.cpp)
target_link_libraries(UpdateMatricesTest osqp-eigen osqp::osqp ${GTEST_LIBRARIES} pthread)
add_test(NAME UpdateMatricesTest
  COMMAND UpdateMatricesTest)

# MPCTest
add_executable(MPCTest MPCTest.cpp)
target_link_libraries(MPCTest osqp-eigen osqp::osqp ${GTEST_LIBRARIES} pthread)
add_test(NAME MPCTest
  COMMAND MPCTest)

# MPCTest update matrix
add_executable(MPCUpdateMatricesTest UpdateMatricesTest.cpp)
target_link_libraries(MPCUpdateMatricesTest osqp-eigen osqp::osqp ${GTEST_LIBRARIES} pthread)
add_test(NAME MPCUpdateMatricesTest
  COMMAND MPCUpdateMatricesTest)
