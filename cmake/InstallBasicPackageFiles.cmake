#.rst:
# InstallBasicPackageFiles
# ------------------------
#
# A helper module to make your package easier to be found by other
# projects.
#
#
# .. command:: install_basic_package_files
#
# Create and install a basic version of cmake config files for your
# project::
#
#  install_basic_package_files(<Name>
#                              VERSION <version>
#                              COMPATIBILITY <compatibility>
#                              TARGETS <target1> <target2>
#                              TARGETS_PROPERTY <property_name>
#                              TARGETS_PROPERTIES <property1_name> <property2_name>
#                              [NO_SET_AND_CHECK_MACRO]
#                              [NO_CHECK_REQUIRED_COMPONENTS_MACRO]
#                              [VARS_PREFIX <prefix>] # (default = "<name>")
#                              [DESTINATION <destination>]
#                              [NAMESPACE <namespace>] # (default = "<name>::")
#                              [EXTRA_PATH_VARS_SUFFIX path1 [path2 ...]]
#                              [CONFIG_TEMPLATE <file>]
#                              [UPPERCASE_FILENAMES | LOWERCASE_FILENAMES]
#                              [DEPENDENCIES <dependency1> <dependency2>]
#                              [NO_COMPATIBILITY_VARS]
#                             )
#
# Depending on UPPERCASE_FILENAMES and LOWERCASE_FILENAMES, this
# function generates 3 files:
#
#  - ``<Name>ConfigVersion.cmake`` or ``<name>-config-version.cmake``
#  - ``<Name>Config.cmake`` or ``<name>-config.cmake``
#  - ``<Name>Targets.cmake`` or ``<name>-targets.cmake``
#
# If neither ``UPPERCASE_FILENAMES`` nor ``LOWERCASE_FILENAMES`` is
# set, a file ``<Name>ConfigVersion.cmake.in`` or
# ``<name>-config-version.cmake.in`` is searched, and the convention
# is chosed according to the file found. If no file was found, the
# uppercase convention is used.
#
# The ``DEPENDENCIES`` flag can be used to set a list of dependencies
# that will be searched using the :command:`find_dependency` command
# from the :module:`CMakeFindDependencyMacro` module.
# When using a custom template file, the ``@PACKAGE_DEPENDENCIES@``
# string is replaced with the code checking for the dependencies
# specified by this argument.
#
# Each file is generated twice, one for the build directory and one for
# the installation directory.  The ``DESTINATION`` argument can be
# passed to install the files in a location different from the default
# one (``CMake`` on Windows, ``${CMAKE_INSTALL_LIBDIR}/cmake/${name}``
# on other platforms.
#
#
# The ``<Name>ConfigVersion.cmake`` is generated using
# ``write_basic_package_version_file``.  The ``VERSION``,
# ``COMPATIBILITY``, ``NO_SET_AND_CHECK_MACRO``, and
# ``NO_CHECK_REQUIRED_COMPONENTS_MACRO`` are passed to this function
# and are used internally by :module:`CMakePackageConfigHelpers` module.
#
# ``VERSION`` shall be in the form ``<major>[.<minor>[.<patch>[.<tweak>]]]]``.
# If no ``VERSION`` is given, the ``PROJECT_VERSION`` variable is used.
# If this hasn’t been set, it errors out.  The ``VERSION`` argument is also used
# to replace the ``@PACKAGE_VERSION@`` string in the configuration file.
#
# ``COMPATIBILITY`` shall be any of ``<AnyNewerVersion|SameMajorVersion|
# ExactVersion>``.
# The ``COMPATIBILITY`` mode ``AnyNewerVersion`` means that the installed
# package version will be considered compatible if it is newer or exactly the
# same as the requested version. This mode should be used for packages which are
# fully backward compatible, also across major versions.
# If ``SameMajorVersion`` is used instead, then the behaviour differs from
# ``AnyNewerVersion`` in that the major version number must be the same as
# requested, e.g. version 2.0 will not be considered compatible if 1.0 is
# requested. This mode should be used for packages which guarantee backward
# compatibility within the same major version. If ``ExactVersion`` is used, then
# the package is only considered compatible if the requested version matches
# exactly its own version number (not considering the tweak version). For
# example, version 1.2.3 of a package is only considered compatible to requested
# version 1.2.3. This mode is for packages without compatibility guarantees. If
# your project has more elaborated version matching rules, you will need to
# write your own custom ConfigVersion.cmake file instead of using this macro.
#
# By default ``install_basic_package_files`` also generates the two helper
# macros ``set_and_check()`` and ``check_required_components()`` into the
# ``<Name>Config.cmake`` file. ``set_and_check()`` should be used instead of the
# normal set() command for setting directories and file locations. Additionally
# to setting the variable it also checks that the referenced file or directory
# actually exists and fails with a ``FATAL_ERROR`` otherwise. This makes sure
# that the created ``<Name>Config.cmake`` file does not contain wrong
# references. When using the ``NO_SET_AND_CHECK_MACRO, this macro is not
# generated into the ``<Name>Config.cmake`` file.
#
# By default, ``install_basic_package_files`` append a call to
# ``check_required_components(<Name>)`` in <Name>Config.cmake file if the
# package supports components. This macro checks whether all requested,
# non-optional components have been found, and if this is not the case, sets the
# ``<Name>_FOUND`` variable to ``FALSE``, so that the package is considered to
# be not found. It does that by testing the ``<Name>_<Component>_FOUND``
# variables for all requested required components. When using the
# ``NO_CHECK_REQUIRED_COMPONENTS_MACRO`` option, this macro is not generated
# into the <Name>Config.cmake file.
#
# Finally, the files in the build and install directory are exactly the same.
#
# See the documentation of :module:`CMakePackageConfigHelpers` module for
# further information and references therein.
#
#
# The ``<Name>Config.cmake`` is generated using
# ``configure_package_config_file``.  See the documentation for the
# :module:`CMakePackageConfigHelpers` module for further information.
# If the ``CONFIG_TEMPLATE`` argument is passed, the specified file
# is used as template for generating the configuration file, otherwise
# this module expects to find a ``<Name>Config.cmake.in`` or
# ``<name>-config.cmake.in`` file either in the root directory of the
# project or in current source directory.
# If the file does not exist, a very basic file is created.
#
# A set of variables are checked and passed to
# ``configure_package_config_file`` as ``PATH_VARS``. For each of the
# ``SUFFIX`` considered, if one of the variables::
#
#     <VARS_PREFIX>_(BUILD|INSTALL)_<SUFFIX>
#     (BUILD|INSTALL)_<VARS_PREFIX>_<SUFFIX>
#
# is defined, the ``<VARS_PREFIX>_<SUFFIX>`` variable will be defined
# before configuring the package.  In order to use that variable in the
# config file, you have to add a line::
#
#   set_and_check(<VARS_PREFIX>_<SUFFIX> \"@PACKAGE_<VARS_PREFIX>_<SUFFIX>@\")
#
# if the path must exist or just::
#
#   set(<VARS_PREFIX>_<SUFFIX> \"@PACKAGE_<VARS_PREFIX>_<SUFFIX>@\")
#
# if the path could be missing.
#
# These variable will have different values whether you are using the
# package from the build tree or from the install directory.  Also these
# files will contain only relative paths, meaning that you can move the
# whole installation and the CMake files will still work.
#
# Default ``PATH_VARS`` suffixes are::
#
#   BINDIR          BIN_DIR
#   SBINDIR         SBIN_DIR
#   LIBEXECDIR      LIBEXEC_DIR
#   SYSCONFDIR      SYSCONF_DIR
#   SHAREDSTATEDIR  SHAREDSTATE_DIR
#   LOCALSTATEDIR   LOCALSTATE_DIR
#   LIBDIR          LIB_DIR
#   INCLUDEDIR      INCLUDE_DIR
#   OLDINCLUDEDIR   OLDINCLUDE_DIR
#   DATAROOTDIR     DATAROOT_DIR
#   DATADIR         DATA_DIR
#   INFODIR         INFO_DIR
#   LOCALEDIR       LOCALE_DIR
#   MANDIR          MAN_DIR
#   DOCDIR          DOC_DIR
#
# more suffixes can be added using the ``EXTRA_PATH_VARS_SUFFIX``
# argument.
#
#
# The ``<name>Targets.cmake`` is generated using
# :command:`export(TARGETS)` in the build tree and
# :command:`install(EXPORT)` in the installation directory.
# The targets are exported using the value for the ``NAMESPACE``
# argument as namespace.
# The targets can be passed using the `TARGETS` argument or using a global
# property, that can be passed to the function using the ``TARGETS_PROPERTY``
# argument
#
# If the ``NO_COMPATIBILITY_VARS`` argument is not set, the compatibility
# variables ``<VARS_PREFIX>_LIBRARIES`` and ``<VARS_PREFIX>_INCLUDE_DIRS``
# are set, trying to guess their correct values from the variables set or
# from the arguments passed to this command. This argument is ignored if
# the template file is not generated by this command.

#=============================================================================
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


if(COMMAND install_basic_package_files)
  return()
endif()


include(GNUInstallDirs)
include(CMakePackageConfigHelpers)
include(CMakeParseArguments)


function(INSTALL_BASIC_PACKAGE_FILES _Name)

  # TODO check that _Name does not contain "-" characters

  set(_options NO_SET_AND_CHECK_MACRO
    NO_CHECK_REQUIRED_COMPONENTS_MACRO
    UPPERCASE_FILENAMES
    LOWERCASE_FILENAMES
    NO_COMPATIBILITY_VARS)
  set(_oneValueArgs VERSION
    COMPATIBILITY
    TARGETS_PROPERTY
    VARS_PREFIX
    DESTINATION
    NAMESPACE
    CONFIG_TEMPLATE)
  set(_multiValueArgs EXTRA_PATH_VARS_SUFFIX
    TARGETS
    TARGETS_PROPERTIES
    DEPENDENCIES)
  cmake_parse_arguments(_IBPF "${_options}" "${_oneValueArgs}" "${_multiValueArgs}" "${ARGN}")

  if(NOT DEFINED _IBPF_VARS_PREFIX)
    set(_IBPF_VARS_PREFIX ${_Name})
  endif()

  if(NOT DEFINED _IBPF_VERSION)
    message(FATAL_ERROR "VERSION argument is required")
  endif()

  if(NOT DEFINED _IBPF_COMPATIBILITY)
    message(FATAL_ERROR "COMPATIBILITY argument is required")
  endif()

  if(NOT DEFINED _IBPF_TARGETS AND
      NOT DEFINED _IBPF_TARGETS_PROPERTY AND
      NOT DEFINED _IBPF_TARGETS_PROPERTIES)
    message(FATAL_ERROR "TARGETS_PROPERTY or TARGET_PROPERTIES argument is required")
  endif()

  if((DEFINED _IBPF_TARGETS AND (DEFINED _IBPF_TARGETS_PROPERTY OR DEFINED _IBPF_TARGETS_PROPERTIES)) OR
      (DEFINED _IBPF_TARGETS_PROPERTY AND DEFINED _IBPF_TARGETS_PROPERTIES))
    message(FATAL_ERROR "Only one argument between TARGETS, TARGETS_PROPERTY, and TARGET_PROPERTIES can be used")
  endif()

  if(_IBPF_UPPERCASE_FILENAMES AND _IBPF_LOWERCASE_FILENAMES)
    message(FATAL_ERROR "UPPERCASE_FILENAMES and LOWERCASE_FILENAMES arguments cannot be used together")
  endif()

  # Path for installed cmake files
  if(NOT DEFINED _IBPF_DESTINATION)
    if(WIN32 AND NOT CYGWIN)
      set(_IBPF_DESTINATION CMake)
    else()
      set(_IBPF_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/${_Name})
    endif()
  endif()

  if(NOT DEFINED _IBPF_NAMESPACE)
    set(_IBPF_NAMESPACE "${_Name}::")
  endif()

  if(_IBPF_NO_SET_AND_CHECK_MACRO)
    list(APPEND configure_package_config_file_extra_args NO_SET_AND_CHECK_MACRO)
  endif()

  if(_IBPF_NO_CHECK_REQUIRED_COMPONENTS_MACRO)
    list(APPEND configure_package_config_file_extra_args NO_CHECK_REQUIRED_COMPONENTS_MACRO)
  endif()


  # Get targets from TARGETS argument or from GLOBAL PROPERTY/PROPERTIES set
  # in TARGETS_PROPERTY or TARGETS_PROPERTIES arguments
  if(DEFINED _IBPF_TARGETS)
    set(_targets ${_IBPF_TARGETS})
  elseif(DEFINED _IBPF_TARGETS_PROPERTY)
    get_property(_targets GLOBAL PROPERTY ${_IBPF_TARGETS_PROPERTY})
  else()
    set(_targets)
    foreach(_prop ${_IBPF_TARGETS_PROPERTIES})
      get_property(_prop_val GLOBAL PROPERTY ${_prop})
      list(APPEND _targets ${_prop_val})
    endforeach()
  endif()
  list(GET _targets 0 _first_target)


  # Set input file for config, and ensure that _IBPF_UPPERCASE_FILENAMES
  # and _IBPF_LOWERCASE_FILENAMES are set correctly
  unset(_config_cmake_in)
  set(_generate_file 0)
  if(DEFINED _IBPF_CONFIG_TEMPLATE)
    if(NOT EXISTS "${_IBPF_CONFIG_TEMPLATE}")
      message(FATAL_ERROR "Config template file \"${_IBPF_CONFIG_TEMPLATE}\" not found")
    endif()
    set(_config_cmake_in "${_IBPF_CONFIG_TEMPLATE}")
    if(NOT _IBPF_UPPERCASE_FILENAMES AND NOT _IBPF_LOWERCASE_FILENAMES)
      if("${_IBPF_CONFIG_TEMPLATE}" MATCHES "${_Name}Config.cmake.in")
        set(_IBPF_UPPERCASE_FILENAMES 1)
      elseif("${_IBPF_CONFIG_TEMPLATE}" MATCHES "${_name}-config.cmake.in")
        set(_IBPF_LOWERCASE_FILENAMES 1)
      else()
        set(_IBPF_UPPERCASE_FILENAMES 1)
      endif()
    endif()
  else()
    string(TOLOWER "${_Name}" _name)
    if(EXISTS "${CMAKE_SOURCE_DIR}/${_Name}Config.cmake.in")
      set(_config_cmake_in "${CMAKE_SOURCE_DIR}/${_Name}Config.cmake.in")
      if(NOT _IBPF_UPPERCASE_FILENAMES AND NOT _IBPF_LOWERCASE_FILENAMES)
        set(_IBPF_UPPERCASE_FILENAMES 1)
      endif()
    elseif(EXISTS "${CMAKE_SOURCE_DIR}/${_name}-config.cmake.in")
      set(_config_cmake_in "${CMAKE_SOURCE_DIR}/${_name}-config.cmake.in")
      if(NOT _IBPF_UPPERCASE_FILENAMES AND NOT _IBPF_LOWERCASE_FILENAMES)
        set(_IBPF_LOWERCASE_FILENAMES 1)
      endif()
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_Name}Config.cmake.in")
      set(_config_cmake_in "${CMAKE_CURRENT_SOURCE_DIR}/${_Name}Config.cmake.in")
      if(NOT _IBPF_UPPERCASE_FILENAMES AND NOT _IBPF_LOWERCASE_FILENAMES)
        set(_IBPF_UPPERCASE_FILENAMES 1)
      endif()
    elseif(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/${_name}-config.cmake.in")
      set(_config_cmake_in "${CMAKE_CURRENT_SOURCE_DIR}/${_name}-config.cmake.in")
      if(NOT _IBPF_UPPERCASE_FILENAMES AND NOT _IBPF_LOWERCASE_FILENAMES)
        set(_IBPF_LOWERCASE_FILENAMES 1)
      endif()
    else()
      set(_generate_file 1)
      if(_IBPF_LOWERCASE_FILENAMES)
        set(_config_cmake_in "${CMAKE_CURRENT_BINARY_DIR}/${_name}-config.cmake")
      else()
        set(_config_cmake_in "${CMAKE_CURRENT_BINARY_DIR}/${_Name}Config.cmake.in")
        set(_IBPF_UPPERCASE_FILENAMES 1)
      endif()
    endif()
  endif()


  # Select output file names
  if(_IBPF_UPPERCASE_FILENAMES)
    set(_config_filename ${_Name}Config.cmake)
    set(_version_filename ${_Name}ConfigVersion.cmake)
    set(_targets_filename ${_Name}Targets.cmake)
  elseif(_IBPF_LOWERCASE_FILENAMES)
    set(_config_filename ${_name}-config.cmake)
    set(_version_filename ${_name}-config-version.cmake)
    set(_targets_filename ${_name}-targets.cmake)
  endif()


  # If the template config file does not exist, write a basic one
  if(_generate_file)
    # Generate the compatibility code
    unset(_compatibility_vars)
    if(NOT _IBPF_NO_COMPATIBILITY_VARS)
      unset(_get_include_dir_code)
      unset(_set_include_dir_code)
      unset(_target_list)
      foreach(_target ${_targets})
        list(APPEND _target_list ${_IBPF_NAMESPACE}${_target})
      endforeach()
      if(DEFINED ${_IBPF_VARS_PREFIX}_BUILD_INCLUDEDIR OR
          DEFINED BUILD_${_IBPF_VARS_PREFIX}_INCLUDEDIR OR
          DEFINED ${_IBPF_VARS_PREFIX}_INSTALL_INCLUDEDIR OR
          DEFINED INSTALL_${_IBPF_VARS_PREFIX}_INCLUDEDIR)
        set(_get_include_dir "set(${_IBPF_VARS_PREFIX}_INCLUDEDIR \"\@PACKAGE_${_IBPF_VARS_PREFIX}_INCLUDEDIR\@\")\n")
        set(_set_include_dir "set(${_Name}_INCLUDE_DIRS \"\${${_IBPF_VARS_PREFIX}_INCLUDEDIR}\")")
      elseif(DEFINED ${_IBPF_VARS_PREFIX}_BUILD_INCLUDE_DIR OR
          DEFINED BUILD_${_IBPF_VARS_PREFIX}_INCLUDE_DIR OR
          DEFINED ${_IBPF_VARS_PREFIX}_INSTALL_INCLUDE_DIR OR
          DEFINED INSTALL_${_IBPF_VARS_PREFIX}_INCLUDE_DIR)
        set(_get_include_dir "set(${_IBPF_VARS_PREFIX}_INCLUDE_DIR \"\@PACKAGE_${_IBPF_VARS_PREFIX}_INCLUDE_DIR\@\")\n")
        set(_set_include_dir "set(${_Name}_INCLUDE_DIRS \"\${${_IBPF_VARS_PREFIX}_INCLUDE_DIR}\")")
      else()
        unset(_include_dir_list)
        foreach(_target ${_targets})
          set(_get_include_dir "${_get_include_dir}get_property(${_IBPF_VARS_PREFIX}_${_target}_INCLUDE_DIR TARGET ${_IBPF_NAMESPACE}${_target} PROPERTY INTERFACE_INCLUDE_DIRECTORIES)\n")
          list(APPEND _include_dir_list "\"\${${_IBPF_VARS_PREFIX}_${_target}_INCLUDE_DIR}\"")
        endforeach()
        string(REPLACE ";" " " _include_dir_list "${_include_dir_list}")
        string(REPLACE ";" " " _target_list "${_target_list}")
        set(_set_include_dir "set(${_Name}_INCLUDE_DIRS ${_include_dir_list})\nlist(REMOVE_DUPLICATES ${_Name}_INCLUDE_DIRS)")
      endif()
      set(_compatibility_vars "# Compatibility\n${_get_include_dir}\nset(${_Name}_LIBRARIES ${_target_list})\n${_set_include_dir}")
    endif()

    # Write the file
    file(WRITE "${_config_cmake_in}"
      "set(${_IBPF_VARS_PREFIX}_VERSION \@PACKAGE_VERSION\@)

\@PACKAGE_INIT\@

\@PACKAGE_DEPENDENCIES\@

if(NOT TARGET ${_IBPF_NAMESPACE}${_first_target})
  include(\"\${CMAKE_CURRENT_LIST_DIR}/${_targets_filename}\")
endif()

${_compatibility_vars}
")
  endif()

  # Make relative paths absolute (needed later on) and append the
  # defined variables to _(build|install)_path_vars_suffix
  foreach(p BINDIR          BIN_DIR
      SBINDIR         SBIN_DIR
      LIBEXECDIR      LIBEXEC_DIR
      SYSCONFDIR      SYSCONF_DIR
      SHAREDSTATEDIR  SHAREDSTATE_DIR
      LOCALSTATEDIR   LOCALSTATE_DIR
      LIBDIR          LIB_DIR
      INCLUDEDIR      INCLUDE_DIR
      OLDINCLUDEDIR   OLDINCLUDE_DIR
      DATAROOTDIR     DATAROOT_DIR
      DATADIR         DATA_DIR
      INFODIR         INFO_DIR
      LOCALEDIR       LOCALE_DIR
      MANDIR          MAN_DIR
      DOCDIR          DOC_DIR
      ${_IBPF_EXTRA_PATH_VARS_SUFFIX})
    if(DEFINED ${_IBPF_VARS_PREFIX}_BUILD_${p})
      list(APPEND _build_path_vars_suffix ${p})
      list(APPEND _build_path_vars "${_IBPF_VARS_PREFIX}_${p}")
    endif()
    if(DEFINED BUILD_${_IBPF_VARS_PREFIX}_${p})
      list(APPEND _build_path_vars_suffix ${p})
      list(APPEND _build_path_vars "${_IBPF_VARS_PREFIX}_${p}")
    endif()
    if(DEFINED ${_IBPF_VARS_PREFIX}_INSTALL_${p})
      list(APPEND _install_path_vars_suffix ${p})
      list(APPEND _install_path_vars "${_IBPF_VARS_PREFIX}_${p}")
    endif()
    if(DEFINED INSTALL_${_IBPF_VARS_PREFIX}_${p})
      list(APPEND _install_path_vars_suffix ${p})
      list(APPEND _install_path_vars "${_IBPF_VARS_PREFIX}_${p}")
    endif()
  endforeach()


  # <name>ConfigVersion.cmake file (same for build tree and intall)
  write_basic_package_version_file("${CMAKE_BINARY_DIR}/${_version_filename}"
    VERSION ${_IBPF_VERSION}
    COMPATIBILITY ${_IBPF_COMPATIBILITY})
  install(FILES "${CMAKE_BINARY_DIR}/${_version_filename}"
    DESTINATION ${_IBPF_DESTINATION})


  # Prepare PACKAGE_DEPENDENCIES variable
  unset(PACKAGE_DEPENDENCIES)
  if(DEFINED _IBPF_DEPENDENCIES)
    set(PACKAGE_DEPENDENCIES "#### Expanded from @PACKAGE_DEPENDENCIES@ by install_basic_package_files() ####\n\ninclude(CMakeFindDependencyMacro)\n")
    foreach(_dep ${_IBPF_DEPENDENCIES})
      set(PACKAGE_DEPENDENCIES "${PACKAGE_DEPENDENCIES}find_dependency(${_dep})\n")
    endforeach()
    set(PACKAGE_DEPENDENCIES "${PACKAGE_DEPENDENCIES}\n###############################################################################\n")
  endif()

  # Prepare PACKAGE_VERSION variable
  set(PACKAGE_VERSION ${_IBPF_VERSION})

  # <name>Config.cmake (build tree)
  foreach(p ${_build_path_vars_suffix})
    if(DEFINED ${_IBPF_VARS_PREFIX}_BUILD_${p})
      set(${_IBPF_VARS_PREFIX}_${p} "${${_IBPF_VARS_PREFIX}_BUILD_${p}}")
    elseif(DEFINED BUILD_${_IBPF_VARS_PREFIX}_${p})
      set(${_IBPF_VARS_PREFIX}_${p} "${BUILD_${_IBPF_VARS_PREFIX}_${p}}")
    endif()
  endforeach()
  configure_package_config_file("${_config_cmake_in}"
    "${CMAKE_BINARY_DIR}/${_config_filename}"
    INSTALL_DESTINATION ${CMAKE_BINARY_DIR}
    PATH_VARS ${_build_path_vars}
    ${configure_package_config_file_extra_args}
    INSTALL_PREFIX ${CMAKE_BINARY_DIR})

  # <name>Config.cmake (installed)
  foreach(p ${_install_path_vars_suffix})
    if(DEFINED ${_IBPF_VARS_PREFIX}_INSTALL_${p})
      set(${_IBPF_VARS_PREFIX}_${p} "${${_IBPF_VARS_PREFIX}_INSTALL_${p}}")
    elseif(DEFINED INSTALL_${_IBPF_VARS_PREFIX}_${p})
      set(${_IBPF_VARS_PREFIX}_${p} "${INSTALL_${_IBPF_VARS_PREFIX}_${p}}")
    endif()
  endforeach()
  configure_package_config_file("${_config_cmake_in}"
    "${CMAKE_CURRENT_BINARY_DIR}/${_config_filename}.install"
    INSTALL_DESTINATION ${_IBPF_DESTINATION}
    PATH_VARS ${_install_path_vars}
    ${configure_package_config_file_extra_args})
  install(FILES "${CMAKE_CURRENT_BINARY_DIR}/${_config_filename}.install"
    DESTINATION ${_IBPF_DESTINATION}
    RENAME ${_config_filename})


  # <name>Targets.cmake (build tree)
  export(TARGETS ${_targets}
    NAMESPACE ${_IBPF_NAMESPACE}
    FILE "${CMAKE_BINARY_DIR}/${_targets_filename}")

  # <name>Targets.cmake (installed)
  install(EXPORT ${_Name}
    NAMESPACE ${_IBPF_NAMESPACE}
    DESTINATION ${_IBPF_DESTINATION}
    FILE "${_targets_filename}")
endfunction()
