# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

# Set cmake mimimun version
cmake_minimum_required(VERSION 3.1)

project(OSPQP-Wrapper)

# Enable C++11
set(CMAKE_CXX_STANDARD 11)

# find Eigen
find_package(Eigen3 REQUIRED)

# add OSQP library
set(OSQP_FOLDER "/usr/lib" CACHE STRING "folder where the OSQP library is installed")
add_library(OSQP_LIB SHARED IMPORTED)
set_target_properties(OSQP_LIB PROPERTIES
  IMPORTED_LOCATION "${OSQP_FOLDER}/build/out/libosqp.so"
  INTERFACE_INCLUDE_DIRECTORIES "${OSQP_FOLDER}/include")
