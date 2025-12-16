cmake_minimum_required(VERSION 3.15)
project(hw3d LANGUAGES CXX)

# cmake -S . -B build -G "Visual Studio 17 2022" -A x64 -DCMAKE_INSTALL_PREFIX=./build/install
# cmake --build build --config Release

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# sensible default for single-config generators
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose the type of build." FORCE)
endif()

# Global option: choose character set for the whole project (cached so
# subdirectories can read the same value).
option(USE_MULTI_BYTE_CHARSET "Use Multi-Byte Character Set (global)" ON)

# If requested, add a global compile definition so all targets inherit it.
if(USE_MULTI_BYTE_CHARSET)
	add_compile_definitions(_MBCS)
endif()

# Make the repository root available as an include directory so headers
# placed at the project root can be included directly. This makes the
# repository root act as the 'include' root.
include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(hw3d)

# If a `demo` folder exists with its own CMakeLists, include it.
if(EXISTS "${CMAKE_SOURCE_DIR}/demo/CMakeLists.txt")
	add_subdirectory(demo)
endif()
