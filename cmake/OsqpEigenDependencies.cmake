# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT). All rights reserved.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

include(OsqpEigenFindOptionalDependencies)

#---------------------------------------------
## Required Dependencies
## osqp-eigen does not have any option to call FetchContent for its dependencies,
## but we support manually calling FetchContent for Eigen3 or osqp before FetchContent
## is called for OsqpEigen, see https://github.com/robotology/osqp-eigen/issues/210
if(NOT TARGET Eigen3::Eigen)
  find_package(Eigen3 3.2.92 REQUIRED)
endif()
if(NOT TARGET osqp::osqp AND NOT TARGET osqp::osqpstatic)
  find_package(osqp REQUIRED)
endif()

# Select the osqp target to link, see https://github.com/robotology/osqp-eigen/issues/196
# Advanced users can directly (for example package manager mantainers) can set the 
# OSQP_EIGEN_OSQP_TARGET_TO_LINK variable to explicitly select the osqp cmake imported target to link
if(NOT DEFINED OSQP_EIGEN_OSQP_TARGET_TO_LINK)
  # In case BUILD_SHARED_LIBS is OFF and osqp::osqpstatic is defined, let's link osqp::osqpstatic
  if(NOT BUILD_SHARED_LIBS AND TARGET osqp::osqpstatic)
    set(OSQP_EIGEN_OSQP_TARGET_TO_LINK osqp::osqpstatic)
  # Otherwise (i.e. BUILD_SHARED_LIBS is ON) if osqp::osqpstatic is defined and osqp::osqp is not, we link
  # osqp::osqpstatic
  elseif(TARGET osqp::osqpstatic AND NOT TARGET osqp::osqp)
    set(OSQP_EIGEN_OSQP_TARGET_TO_LINK osqp::osqpstatic)
  # In all other cases we link osqp::osqp 
  else()
    set(OSQP_EIGEN_OSQP_TARGET_TO_LINK osqp::osqp)
  endif()

  # If no global OSQP_EIGEN_OSQP_TARGET_TO_LINK is defined, we make sure that the variable is defined also in the parent scope
  set(OSQP_EIGEN_OSQP_TARGET_TO_LINK ${OSQP_EIGEN_OSQP_TARGET_TO_LINK} PARENT_SCOPE)
endif()

# OSQP_IS_V1 (and OSQP_EIGEN_OSQP_IS_V1) is defined for v1.0.0.beta1 and v1.0.0 (and later)
if(NOT DEFINED OSQP_IS_V1)
  try_compile(OSQP_IS_V1 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR}/try-osqp-v1.cpp LINK_LIBRARIES ${OSQP_EIGEN_OSQP_TARGET_TO_LINK})
endif()

# OSQP_IS_V1 (and OSQP_EIGEN_OSQP_IS_V1) is defined only for v1.0.0 (and later)
if(NOT DEFINED OSQP_IS_V1_FINAL)
  try_compile(OSQP_IS_V1_FINAL ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR}/try-osqp-v1-final.cpp LINK_LIBRARIES ${OSQP_EIGEN_OSQP_TARGET_TO_LINK})
endif()


#---------------------------------------------
## Optional Dependencies
find_package(Catch2 3.0.1 QUIET)
checkandset_optional_dependency(Catch2)

find_package(VALGRIND QUIET)
checkandset_optional_dependency(VALGRIND)
