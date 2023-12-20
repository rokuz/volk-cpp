# Include the volk-cpp target through add_subdirectory, use the static lib target.
# We must set platform defines.
# By default, Vulkan is pulled in as transitive dependency if found.

cmake_minimum_required(VERSION 3.5)
project(volk_cpp_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Set a suitable platform define to compile volk-cpp with.
if(CMAKE_SYSTEM_NAME STREQUAL Windows)
  set(VOLK_CPP_STATIC_DEFINES VK_USE_PLATFORM_WIN32_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Linux)
  set(VOLK_CPP_STATIC_DEFINES VK_USE_PLATFORM_XLIB_KHR)
elseif(CMAKE_SYSTEM_NAME STREQUAL Darwin)
  set(VOLK_CPP_STATIC_DEFINES VK_USE_PLATFORM_MACOS_MVK)
endif()

# Include volk-cpp as part of the build tree to make the target known.
# The two-argument version of add_subdirectory allows adding non-subdirs.
add_subdirectory(../.. volk-cpp)

add_executable(volk_cpp_test main.cpp)
target_link_libraries(volk_cpp_test PRIVATE volk_cpp)

# Get Vulkan dependency
find_package(Vulkan QUIET)
if(TARGET Vulkan::Vulkan)
  target_link_libraries(volk_cpp_test PRIVATE Vulkan::Vulkan)
elseif(DEFINED ENV{VULKAN_SDK})
  target_include_directories(volk_cpp_test PRIVATE "$ENV{VULKAN_SDK}/include")
endif()
