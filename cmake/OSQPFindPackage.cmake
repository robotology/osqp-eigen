# Authors: Giulio Romualdi
# CopyPolicy: Released under the terms of the LGPLv2.1 or later

# simple helper to find OSQP library
set(OSQP_DIR "/usr/lib" CACHE PATH "Folder where the OSQP library is downloaded.")
add_library(OSQP_LIB SHARED IMPORTED)
set_target_properties(OSQP_LIB PROPERTIES
  IMPORTED_LOCATION "${OSQP_DIR}/lib/libosqp.so"
  INTERFACE_INCLUDE_DIRECTORIES "${OSQP_DIR}/include")
