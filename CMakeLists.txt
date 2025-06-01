cmake_minimum_required(VERSION 3.5)
cmake_policy(PUSH)
cmake_policy(SET CMP0048 NEW) # project(... VERSION ...) support

project(volk_cpp VERSION
# VOLK_CPP_GENERATE_VERSION
316
# VOLK_CPP_GENERATE_VERSION
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# CMake 3.12 changes the default behaviour of option() to leave local variables
# unchanged if they exist (which we want), but we must work with older CMake versions.
if(NOT DEFINED VOLK_CPP_STATIC_DEFINES)
  option(VOLK_CPP_STATIC_DEFINES "Additional defines for building the volk static library, e.g. Vulkan platform defines" "")
endif()
if(NOT DEFINED VOLK_CPP_PULL_IN_VULKAN)
  option(VOLK_CPP_PULL_IN_VULKAN "Vulkan as a transitive dependency" ON)
endif()
if(NOT DEFINED VOLK_CPP_INSTALL)
  option(VOLK_CPP_INSTALL "Create installation targets" OFF)
endif()
if(NOT DEFINED VOLK_CPP_HEADERS_ONLY)
  option(VOLK_CPP_HEADERS_ONLY "Add interface library only" OFF)
endif()
if(NOT DEFINED VULKAN_HEADERS_INSTALL_DIR)
  option(VULKAN_HEADERS_INSTALL_DIR "Where to get the Vulkan headers" "")
endif()

# -----------------------------------------------------
# Static library

if(NOT VOLK_CPP_HEADERS_ONLY OR VOLK_CPP_INSTALL)
  add_library(volk_cpp STATIC volk.hpp volk.cpp)
  add_library(volk::volk_cpp ALIAS volk_cpp)
  target_include_directories(volk_cpp PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
    $<INSTALL_INTERFACE:include>
  )
  if(VOLK_CPP_STATIC_DEFINES)
    target_compile_definitions(volk_cpp PUBLIC ${VOLK_CPP_STATIC_DEFINES})
  endif()
  if (NOT WIN32)
    target_link_libraries(volk_cpp PUBLIC dl)
  endif()
endif()

# -----------------------------------------------------
# Interface library

add_library(volk_cpp_headers INTERFACE)
add_library(volk::volk_cpp_headers ALIAS volk_cpp_headers)
target_include_directories(volk_cpp_headers INTERFACE
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}>
  $<INSTALL_INTERFACE:include>
)
if (NOT WIN32)
  target_link_libraries(volk_cpp_headers INTERFACE dl)
endif()

# -----------------------------------------------------
# Vulkan transitive dependency

if(VOLK_CPP_PULL_IN_VULKAN)
  # If CMake has the FindVulkan module and it works, use it.
  find_package(Vulkan QUIET)

  # Try an explicit CMake variable first, then any Vulkan paths
  # discovered by FindVulkan.cmake, then the $VULKAN_SDK environment
  # variable if nothing else works.
  if(VULKAN_HEADERS_INSTALL_DIR)
    message("volk: using VULKAN_HEADERS_INSTALL_DIR option")
    set(VOLK_CPP_INCLUDES "${VULKAN_HEADERS_INSTALL_DIR}/include")
  elseif(Vulkan_INCLUDE_DIRS)
    message("volk: using Vulkan_INCLUDE_DIRS from FindVulkan module")
    set(VOLK_CPP_INCLUDES "${Vulkan_INCLUDE_DIRS}")
  elseif(DEFINED ENV{VULKAN_SDK})
    message("volk: using VULKAN_SDK environment variable")
    set(VOLK_CPP_INCLUDES "$ENV{VULKAN_SDK}/include")
  endif()

  if(VOLK_CPP_INCLUDES)
    if(TARGET volk_cpp)
      target_include_directories(volk_cpp PUBLIC "${VOLK_CPP_INCLUDES}")
    endif()
    target_include_directories(volk_cpp_headers INTERFACE "${VOLK_CPP_INCLUDES}")
  endif()
endif()

# -----------------------------------------------------
# Installation

if(VOLK_CPP_INSTALL)

  include(GNUInstallDirs)
  set(INSTALL_CONFIGDIR ${CMAKE_INSTALL_LIBDIR}/cmake/volkCpp)

  # Install files
  install(FILES volk.hpp volk.cpp DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

  # Install library target and add it and any dependencies to export set.
  install(TARGETS volk_cpp volk_cpp_headers
    EXPORT volkCpp-targets
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
  )

  # Actually write exported config w/ imported targets
  install(EXPORT volkCpp-targets
    FILE volkCppTargets.cmake
    NAMESPACE volk::
    DESTINATION ${INSTALL_CONFIGDIR}
  )

  # Create a ConfigVersion.cmake file:
  include(CMakePackageConfigHelpers)
  write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/volkCppConfigVersion.cmake
    COMPATIBILITY AnyNewerVersion
  )

  # Configure config file
  configure_package_config_file(${CMAKE_CURRENT_LIST_DIR}/cmake/volkCppConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/volkCppConfig.cmake
    INSTALL_DESTINATION ${INSTALL_CONFIGDIR}
  )

  # Install the fully generated config and configVersion files
  install(FILES
    ${CMAKE_CURRENT_BINARY_DIR}/volkCppConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/volkCppConfigVersion.cmake
    DESTINATION ${INSTALL_CONFIGDIR}
  )

endif()
cmake_policy(POP)
