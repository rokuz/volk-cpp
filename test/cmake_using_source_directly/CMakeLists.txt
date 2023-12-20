# Compiles the volk sources as part of a user project.
# volk-cpp comes with a volk.cpp for this purpose.
# Note that for volk-cpp to properly handle platform defines,
# those have to be set at build time.
# Also note that this way the Vulkan headers must
# handled by the user project as well as linking to dl on
# non-Windows platforms.
# For these reasons it's recommended to use one of
# the other ways to include volk-cpp (see the other examples).

cmake_minimum_required(VERSION 3.5)
project(volk_cpp_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_executable(volk_cpp_test main.cpp ../../volk.cpp)

# Set include path for volk.hpp
target_include_directories(volk_cpp_test PRIVATE ../..)

# Set suitable platform defines
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  target_compile_definitions(volk_cpp_test PRIVATE VK_USE_PLATFORM_WIN32_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  target_compile_definitions(volk_cpp_test PRIVATE VK_USE_PLATFORM_XLIB_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  target_compile_definitions(volk_cpp_test PRIVATE VK_USE_PLATFORM_MACOS_MVK)
endif()

# Link requires libraries
if(NOT WIN32)
  target_link_libraries(volk_cpp_test PRIVATE dl)
endif()

# Get Vulkan dependency
find_package(Vulkan QUIET)
if(TARGET Vulkan::Vulkan)
  target_link_libraries(volk_cpp_test PRIVATE Vulkan::Vulkan)
elseif(DEFINED ENV{VULKAN_SDK})
  target_include_directories(volk_cpp_test PRIVATE "$ENV{VULKAN_SDK}/include")
endif()
