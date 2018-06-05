# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

# Set cmake mimimun version
cmake_minimum_required(VERSION 3.1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

# Enable C++14
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED 11)

project(OsqpEigen
  LANGUAGES CXX
  VERSION 0.3.0)

# add GNU dirs
include(GNUInstallDirs)

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
find_package(osqp REQUIRED)

set(LIBRARY_TARGET_NAME OsqpEigen)

# List of CPP (source) library files.
set(${LIBRARY_TARGET_NAME}_SRC
  src/OptimizerData.cpp
  src/OptimizerSettings.cpp
  src/OptimizerSolver.cpp)

set(${LIBRARY_TARGET_NAME}_HDR
  include/OptimizerConstants.hpp
  include/SparseMatrixHelper.hpp
  include/SparseMatrixHelper.tpp
  include/OptimizerData.hpp
  include/OptimizerData.tpp
  include/OptimizerSettings.hpp
  include/OptimizerSolver.hpp
  include/OptimizerSolver.tpp)

add_library(${LIBRARY_TARGET_NAME} SHARED ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}/OsqpEigen>")

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE osqp::osqp)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

install(TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT  ${PROJECT_NAME}
  COMPONENT runtime
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
  ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
  RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/OsqpEigen")

install(EXPORT ${PROJECT_NAME}
  NAMESPACE OsqpEigen::
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OsqpEigen)

configure_package_config_file(${CMAKE_SOURCE_DIR}/cmake/OsqpEigenConfig.cmake.in
  ${CMAKE_BINARY_DIR}/OsqpEigenConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OsqpEigen)

install(FILES ${CMAKE_BINARY_DIR}/OsqpEigenConfig.cmake DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/OsqpEigen)

option(BUILD_TESTING "Create tests using CMake" ON)

include(AddUninstallTarget)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
