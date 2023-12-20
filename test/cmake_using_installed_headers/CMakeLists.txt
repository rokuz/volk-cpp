cmake_minimum_required(VERSION 3.5)
project(volk_cpp_test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Include volk from a CMake package config.
# CMAKE_PREFIX_PATH or volk_DIR must be set properly.
find_package(volkCpp CONFIG REQUIRED)

add_executable(volk_cpp_test main.cpp)
target_link_libraries(volk_cpp_test PRIVATE volk::volk_cpp_headers)

# Get Vulkan dependency
find_package(Vulkan QUIET)
if(TARGET Vulkan::Vulkan)
  target_link_libraries(volk_cpp_test PRIVATE Vulkan::Vulkan)
elseif(DEFINED ENV{VULKAN_SDK})
  target_include_directories(volk_cpp_test PRIVATE "$ENV{VULKAN_SDK}/include")
endif()
