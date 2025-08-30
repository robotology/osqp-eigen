# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the BSD 3-clause license

cmake_minimum_required(VERSION 3.8...3.30)

project(OsqpEigen-Example)

find_package(OsqpEigen)

#MPCExample
add_executable(MPCExample src/MPCExample.cpp)
target_link_libraries(MPCExample OsqpEigen::OsqpEigen)
