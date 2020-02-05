# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

cmake_minimum_required(VERSION 3.1)

set (CMAKE_CXX_STANDARD 11)

project(OsqpEigen-Test)

if(NOT TARGET OsqpEigen::OsqpEigen)
  find_package(osqp REQUIRED)
  find_package(OsqpEigen REQUIRED)
  find_package(Eigen3 REQUIRED)
  include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})
endif()

find_package(GTest REQUIRED)
include_directories(${GTEST_INCLUDE_DIRS})

# adding support for checking the tests with valgrind
if(RUN_VALGRIND_TESTS)
     find_package(VALGRIND REQUIRED)
     if(VALGRIND_FOUND)
         set(CTEST_MEMORYCHECK_COMMAND ${VALGRIND_PROGRAM})
         set(MEMORYCHECK_COMMAND ${VALGRIND_PROGRAM})
         if (APPLE)
             set(MEMORYCHECK_SUPPRESSIONS "--suppressions=${PROJECT_SOURCE_DIR}/cmake/valgrind-macos.supp")
         else ()
             set(MEMORYCHECK_SUPPRESSIONS "")
         endif ()
         set(MEMORYCHECK_COMMAND_OPTIONS "--leak-check=full --error-exitcode=1 ${MEMORYCHECK_SUPPRESSIONS}"  CACHE STRING "Options to pass to the memory checker")
         mark_as_advanced(MEMORYCHECK_COMMAND_OPTIONS)
         set(MEMCHECK_COMMAND_COMPLETE "${MEMORYCHECK_COMMAND} ${MEMORYCHECK_COMMAND_OPTIONS}")
         separate_arguments(MEMCHECK_COMMAND_COMPLETE)
     endif()
endif()

macro(add_osqpeigen_test classname)
    set(testsrc ${classname}Test.cpp)
    set(testbinary ${classname}Test)
    set(testname   ${classname}Test)
    add_executable(${testbinary} ${testsrc})
    target_link_libraries(${testbinary} PRIVATE OsqpEigen::OsqpEigen osqp::osqp Eigen3::Eigen ${GTEST_LIBRARIES} pthread)
    add_test(NAME ${testname} COMMAND ${testbinary})

    if(RUN_VALGRIND_TESTS)
        add_test(NAME memcheck_${testname} COMMAND ${MEMCHECK_COMMAND_COMPLETE} $<TARGET_FILE:${testbinary}>)
    endif()
endmacro()

# QPTest
add_osqpeigen_test(SparseMatrix)

# QPTest
add_osqpeigen_test(QP)

# Update matrix
add_osqpeigen_test(UpdateMatrices)

# MPCTest
add_osqpeigen_test(MPC)

# MPCTest update matrix
add_osqpeigen_test(MPCUpdateMatrices)
