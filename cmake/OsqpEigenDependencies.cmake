# Copyright (C) 2019 Istituto Italiano di Tecnologia (IIT). All rights reserved.
# This software may be modified and distributed under the terms of the
# GNU Lesser General Public License v2.1 or any later version.
#
# This software may be modified and distributed under the terms of the
# BSD-3-Clause license. See the accompanying LICENSE file for details.

include(OsqpEigenFindOptionalDependencies)

#---------------------------------------------
## Required Dependencies
find_package(Eigen3 3.2.92 REQUIRED)
find_package(osqp REQUIRED)

if(OSQP_EIGEN_LINK_OSQP_SHARED)
  set(OSQP_LIB osqp::osqp)
else()
  set(OSQP_LIB osqp::osqpstatic)
endif()

# OSQP_IS_V1 (and OSQP_EIGEN_OSQP_IS_V1) is defined for v1.0.0.beta1 and v1.0.0 (and later)
if(NOT DEFINED OSQP_IS_V1)
  try_compile(OSQP_IS_V1 ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR}/try-osqp-v1.cpp LINK_LIBRARIES ${OSQP_LIB})
endif()

# OSQP_IS_V1 (and OSQP_EIGEN_OSQP_IS_V1) is defined only for v1.0.0 (and later)
if(NOT DEFINED OSQP_IS_V1_FINAL)
  try_compile(OSQP_IS_V1_FINAL ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_LIST_DIR}/try-osqp-v1-final.cpp LINK_LIBRARIES ${OSQP_LIB})
endif()


#---------------------------------------------
## Optional Dependencies
find_package(Catch2 3.0.1 QUIET)
checkandset_optional_dependency(Catch2)

find_package(VALGRIND QUIET)
checkandset_optional_dependency(VALGRIND)
