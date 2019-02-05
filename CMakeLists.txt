# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

# Set cmake mimimun version
cmake_minimum_required(VERSION 3.1)

# ouptut paths
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_BINDIR}")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${CMAKE_INSTALL_LIBDIR}")

# Build shared libs
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

if(MSVC)
  set(CMAKE_DEBUG_POSTFIX "d")
endif()

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)

option(BUILD_SHARED_LIBS "Build libraries as shared as opposed to static" ON)

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

# Encourage user to specify a build type (e.g. Release, Debug, etc.), otherwise set it to Release.
if(NOT CMAKE_CONFIGURATION_TYPES)
  if(NOT CMAKE_BUILD_TYPE)
      message(STATUS "Setting build type to 'Release' as none was specified.")
      set_property(CACHE CMAKE_BUILD_TYPE PROPERTY VALUE "Release")
  endif()
endif()

# find Eigen
find_package(Eigen3 REQUIRED)
include_directories(SYSTEM ${EIGEN3_INCLUDE_DIR})

# add OSQP library
find_package(osqp REQUIRED)

set(LIBRARY_TARGET_NAME OsqpEigen)

# List of CPP (source) library files.
set(${LIBRARY_TARGET_NAME}_SRC
  src/Data.cpp
  src/Settings.cpp
  src/Solver.cpp)

set(${LIBRARY_TARGET_NAME}_HDR
  include/OsqpEigen.h
  include/OsqpEigen/Constants.hpp
  include/OsqpEigen/SparseMatrixHelper.hpp
  include/OsqpEigen/SparseMatrixHelper.tpp
  include/OsqpEigen/Data.hpp
  include/OsqpEigen/Data.tpp
  include/OsqpEigen/Settings.hpp
  include/OsqpEigen/Solver.hpp
  include/OsqpEigen/Solver.tpp)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${LIBRARY_TARGET_NAME} PRIVATE osqp::osqp Eigen3::Eigen)

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

option(BUILD_TESTING "Create tests using CMake" OFF)

include(AddUninstallTarget)

include(CTest)
if(BUILD_TESTING)
  add_subdirectory(tests)
endif()
