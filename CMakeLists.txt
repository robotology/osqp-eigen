# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the BSD 3-clause license

# Set cmake minimum version
cmake_minimum_required(VERSION 3.8)

# Extract version numbers from package.xml
# Based on code by DART (BSD-2-license)
# Cf. https://github.com/dartsim/dart/blob/f07186fbdb4e89e5340abb9718d02715bb315922/CMakeLists.txt#L62-L68
file(READ package.xml PACKAGE_XML)
string(REGEX MATCH "<version>[0-9]+\\.[0-9]+\\.[0-9]+</version>" DIRTY_VERSION_STRING ${PACKAGE_XML})
string(REGEX REPLACE "^<version>([0-9]+)\\.([0-9]+)\\.([0-9]+)</version>$" "\\1" OsqpEigen_MAJOR_VERSION "${DIRTY_VERSION_STRING}")
string(REGEX REPLACE "^<version>([0-9]+)\\.([0-9]+)\\.([0-9]+)</version>$" "\\2" OsqpEigen_MINOR_VERSION "${DIRTY_VERSION_STRING}")
string(REGEX REPLACE "^<version>([0-9]+)\\.([0-9]+)\\.([0-9]+)</version>$" "\\3" OsqpEigen_PATCH_VERSION "${DIRTY_VERSION_STRING}")
set(OsqpEigen_VERSION "${OsqpEigen_MAJOR_VERSION}.${OsqpEigen_MINOR_VERSION}.${OsqpEigen_PATCH_VERSION}")

# Set project version
project(OsqpEigen VERSION ${OsqpEigen_VERSION})

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

# Disable C and C++ compiler extensions.
# C/CXX_EXTENSIONS are ON by default to allow the compilers to use extended
# variants of the C/CXX language.
# However, this could expose cross-platform bugs in user code or in the headers
# of third-party dependencies and thus it is strongly suggested to turn
# extensions off.
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_EXTENSIONS OFF)

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

option(BUILD_TESTING "Create tests using CMake" OFF)
include(CTest)

option(OSQP_EIGEN_DEBUG_OUTPUT "Print debug error messages to cerr" ON)

# Check OsqpEigen dependencies, find necessary libraries.
include(OsqpEigenDependencies)

# Set default build type to "Release" in single-config generators
if(NOT CMAKE_CONFIGURATION_TYPES)
    if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "Release" CACHE STRING
        "Choose the type of build, recommanded options are: Debug or Release" FORCE)
    endif()
    set(OSQPEIGEN_BUILD_TYPES "Debug" "Release" "MinSizeRel" "RelWithDebInfo")
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS ${OSQPEIGEN_BUILD_TYPES})
endif()

set(LIBRARY_TARGET_NAME OsqpEigen)

# List of CPP (source) library files.
set(${LIBRARY_TARGET_NAME}_SRC
  src/Data.cpp
  src/Settings.cpp
  src/Solver.cpp
  src/Debug.cpp)

set(${LIBRARY_TARGET_NAME}_HDR
  include/OsqpEigen/OsqpEigen.h
  include/OsqpEigen/Constants.hpp
  include/OsqpEigen/SparseMatrixHelper.hpp
  include/OsqpEigen/SparseMatrixHelper.tpp
  include/OsqpEigen/Data.hpp
  include/OsqpEigen/Data.tpp
  include/OsqpEigen/Settings.hpp
  include/OsqpEigen/Solver.hpp
  include/OsqpEigen/Solver.tpp
  include/OsqpEigen/Debug.hpp)

add_library(${LIBRARY_TARGET_NAME} ${${LIBRARY_TARGET_NAME}_SRC} ${${LIBRARY_TARGET_NAME}_HDR})
target_include_directories(${LIBRARY_TARGET_NAME} PUBLIC "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
  "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")

target_link_libraries(${LIBRARY_TARGET_NAME} PUBLIC osqp::osqp Eigen3::Eigen)

add_library(OsqpEigen::OsqpEigen ALIAS OsqpEigen)

set_target_properties(${LIBRARY_TARGET_NAME} PROPERTIES
  VERSION ${PROJECT_VERSION}
  PUBLIC_HEADER "${${LIBRARY_TARGET_NAME}_HDR}")

target_compile_features(${LIBRARY_TARGET_NAME} PUBLIC cxx_std_14)

if(OSQP_EIGEN_DEBUG_OUTPUT)
    target_compile_definitions(${LIBRARY_TARGET_NAME} PRIVATE "OSQP_EIGEN_DEBUG_OUTPUT")
endif()

# List exported CMake package dependencies
set(OSQP_EIGEN_EXPORTED_DEPENDENCIES "")
list(APPEND OSQP_EIGEN_EXPORTED_DEPENDENCIES osqp "Eigen3 CONFIG")

install(TARGETS ${LIBRARY_TARGET_NAME}
  EXPORT  ${PROJECT_NAME}
  COMPONENT runtime
  LIBRARY       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT shlib
  ARCHIVE       DESTINATION "${CMAKE_INSTALL_LIBDIR}"                            COMPONENT lib
  RUNTIME       DESTINATION "${CMAKE_INSTALL_BINDIR}"                            COMPONENT bin
  PUBLIC_HEADER DESTINATION "${CMAKE_INSTALL_INCLUDEDIR}/OsqpEigen")

include(InstallBasicPackageFiles)
install_basic_package_files(${PROJECT_NAME}
  NAMESPACE OsqpEigen::
  VERSION ${${PROJECT_NAME}_VERSION}
  COMPATIBILITY SameMajorVersion
  VARS_PREFIX ${PROJECT_NAME}
  NO_CHECK_REQUIRED_COMPONENTS_MACRO
  DEPENDENCIES ${OSQP_EIGEN_EXPORTED_DEPENDENCIES})

# Install package.xml to share
install(FILES package.xml DESTINATION share/cmake/osqp-eigen)

## Testing
include(AddOsqpEigenUnitTest)
add_subdirectory(tests)

include(AddUninstallTarget)
