# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

# Set cmake mimimun version
cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Enable C++11
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

project(OSPQP-Wrapper
  LANGUAGES CXX)
#  VERSION 0.0.0.0)

#include(GNUInstallDirs)

# include macros for warnings
include(AddWarningsConfigurationToTargets)

include(CMakePackageConfigHelpers)

option(ENABLE_RPATH "Enable RPATH for this library" ON)
mark_as_advanced(ENABLE_RPATH)
include(AddInstallRPATHSupport)
add_install_rpath_support(BIN_DIRS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_BINDIR}"
  LIB_DIRS "${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_LIBDIR}"
  DEPENDS ENABLE_RPATH
  USE_LINK_PATH)

# find Eigen
find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

# add OSQP library
set(OSQP_FOLDER "/usr/lib" CACHE STRING "folder where the OSQP library is installed")
add_library(OSQP_LIB SHARED IMPORTED)
set_target_properties(OSQP_LIB PROPERTIES
  IMPORTED_LOCATION "${OSQP_FOLDER}/build/out/libosqp.so"
  INTERFACE_INCLUDE_DIRECTORIES "${OSQP_FOLDER}/include")

set(LIBRARY_TARGET_NAME OSQPWrapper)


# List of CPP (source) library files.
set(${LIBRARY_TARGET_NAME}_SRC
  src/SparseMatrix.cpp
  src/OptimizatorData.cpp)

set(${LIBRARY_TARGET_NAME}_HDR
  include/SparseMatrix.hpp
  include/SparseMatrix.tpp
  include/OptimizatorData.hpp
  include/OptimizatorData.tpp)

add_library(${LIBRARY_TARGET_NAME} SHARED ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${LIBRARY_TARGET_NAME} OSQP_LIB)
set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
  VERSION 1
  PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}"
  LINKER_LANGUAGE CXX)

install(TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT  ${PROJECT_NAME}
  COMPONENT runtime
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
  ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
  RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}")

install(EXPORT ${PROJECT_NAME}
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OSQPWrapper)

configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/OSQPWrapperConfig.cmake.in
  ${CMAKE_BINARY_DIR}/OSQPWrapperConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OSQPWrapperConfig)

install(FILES ${CMAKE_BINARY_DIR}/OSQPWrapperConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OSQPWrapper)

option(BUILD_TESTING "Create tests using CMake" OFF)

include(AddUninstallTarget)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
