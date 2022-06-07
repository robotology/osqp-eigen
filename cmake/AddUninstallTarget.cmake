#.rst:
# AddUninstallTarget
# ------------------
#
# Add the "uninstall" target for your project::
#
#   include(AddUninstallTarget)
#
#
# will create a file ``cmake_uninstall.cmake`` in the build directory and add a
# custom target ``uninstall`` (or ``UNINSTALL`` on Visual Studio and Xcode) that
# will remove the files installed by your package (using
# ``install_manifest.txt``).
# See also
# https://gitlab.kitware.com/cmake/community/wikis/FAQ#can-i-do-make-uninstall-with-cmake
#
# The :module:`AddUninstallTarget` module must be included in your main
# ``CMakeLists.txt``. If included in a subdirectory it does nothing.
# This allows you to use it safely in your main ``CMakeLists.txt`` and include
# your project using ``add_subdirectory`` (for example when using it with
# :cmake:module:`FetchContent`).
#
# If the ``uninstall`` target already exists, the module does nothing.

#=============================================================================
# Copyright 2008-2013 Kitware, Inc.
# Copyright 2013 Istituto Italiano di Tecnologia (IIT)
#   Authors: Daniele E. Domenichelli <daniele.domenichelli@iit.it>
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distribute this file outside of CMake, substitute the full
#  License text for the above reference.)


# AddUninstallTarget works only when included in the main CMakeLists.txt
if(NOT "${CMAKE_CURRENT_BINARY_DIR}" STREQUAL "${CMAKE_BINARY_DIR}")
  return()
endif()

# The name of the target is uppercase in MSVC and Xcode (for coherence with the
# other standard targets)
if("${CMAKE_GENERATOR}" MATCHES "^(Visual Studio|Xcode)")
  set(_uninstall "UNINSTALL")
else()
  set(_uninstall "uninstall")
endif()

# If target is already defined don't do anything
if(TARGET ${_uninstall})
  return()
endif()


set(_filename "${CMAKE_CURRENT_BINARY_DIR}/cmake_uninstall.cmake")

file(WRITE ${_filename}
"if(NOT EXISTS \"${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt\")
  message(WARNING \"Cannot find install manifest: \\\"${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt\\\"\")
  return()
endif()
file(READ \"${CMAKE_CURRENT_BINARY_DIR}/install_manifest.txt\" files)
string(STRIP \"\${files}\" files)
string(REGEX REPLACE \"\\n\" \";\" files \"\${files}\")
list(REVERSE files)
foreach(file \${files})
  if(IS_SYMLINK \"\$ENV{DESTDIR}\${file}\" OR EXISTS \"\$ENV{DESTDIR}\${file}\")
    message(STATUS \"Uninstalling: \$ENV{DESTDIR}\${file}\")
    execute_process(
      COMMAND \${CMAKE_COMMAND} -E remove \"\$ENV{DESTDIR}\${file}\"
      OUTPUT_VARIABLE rm_out
      RESULT_VARIABLE rm_retval)
    if(NOT \"\${rm_retval}\" EQUAL 0)
      message(FATAL_ERROR \"Problem when removing \\\"\$ENV{DESTDIR}\${file}\\\"\")
    endif()
  else()
    message(STATUS \"Not-found: \$ENV{DESTDIR}\${file}\")
  endif()
endforeach(file)
")

add_custom_target(${_uninstall}
                  COMMAND "${CMAKE_COMMAND}" -E cmake_echo_color --switch=$\(COLOR\) --cyan "Uninstall the project..."
                  COMMAND "${CMAKE_COMMAND}" -P "${_filename}")
set_property(TARGET ${_uninstall} PROPERTY FOLDER "CMakePredefinedTargets")
