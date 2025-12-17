# CMake project for hw3d
cmake_minimum_required(VERSION 3.15)

project(hw3d_project LANGUAGES CXX)

# Options (read global `USE_MULTI_BYTE_CHARSET` from top-level CMake)
option(BUILD_HW3D_SHARED "Build hw3d as a shared library" OFF)
option(BUILD_HW3D_STATIC "Build hw3d as a static library" ON)

# output layout
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# collect sources
file(GLOB HW3D_ALL_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.rc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.inl"
)

# derive library sources (all sources in this dir)
set(LIB_SOURCES ${HW3D_ALL_SOURCES})

# filter out IDE artifact files if present
list(FILTER LIB_SOURCES EXCLUDE REGEX ".*\\.aps$")

# Create libraries conditionally
if(BUILD_HW3D_SHARED)
  add_library(hw3d_shared SHARED ${LIB_SOURCES})
  target_include_directories(hw3d_shared PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
  target_link_libraries(hw3d_shared PRIVATE user32 gdi32)
  if(MSVC)
    set_target_properties(hw3d_shared PROPERTIES WINDOWS_EXPORT_ALL_SYMBOLS ON)
  endif()
endif()

if(BUILD_HW3D_STATIC)
  add_library(hw3d_static STATIC ${LIB_SOURCES})
  target_include_directories(hw3d_static PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
endif()

# common compile options for all created targets
if(MSVC)
  if(TARGET hw3d_shared)
    target_compile_options(hw3d_shared PRIVATE /W4 /permissive-)
  endif()
  if(TARGET hw3d_static)
    target_compile_options(hw3d_static PRIVATE /W4 /permissive-)
  endif()
else()
  if(TARGET hw3d_shared)
    target_compile_options(hw3d_shared PRIVATE -Wall -Wextra -Wpedantic)
  endif()
  if(TARGET hw3d_static)
    target_compile_options(hw3d_static PRIVATE -Wall -Wextra -Wpedantic)
  endif()
endif()

# Apply MBCS option to built targets
if(USE_MULTI_BYTE_CHARSET)
  foreach(_tgt hw3d_shared hw3d_static)
    if(TARGET ${_tgt})
      target_compile_definitions(${_tgt} PRIVATE _MBCS)
      if(CMAKE_GENERATOR MATCHES "Visual Studio")
        set_target_properties(${_tgt} PROPERTIES VS_GLOBAL_CharacterSet "MultiByte")
      endif()
    endif()
  endforeach()
endif()

# Installation
if(NOT CMAKE_INSTALL_PREFIX)
  set(CMAKE_INSTALL_PREFIX "${CMAKE_BINARY_DIR}/install" CACHE PATH "Install path" FORCE)
endif()

if(TARGET hw3d_static)
  install(TARGETS hw3d_static
          ARCHIVE DESTINATION lib
          PUBLIC_HEADER DESTINATION include/hw3d)
endif()

if(TARGET hw3d_shared)
  install(TARGETS hw3d_shared
          RUNTIME DESTINATION bin
          LIBRARY DESTINATION lib
          ARCHIVE DESTINATION lib
          PUBLIC_HEADER DESTINATION include/hw3d)
endif()

# Install public headers from the hw3d directory
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/
        DESTINATION include/hw3d
        FILES_MATCHING PATTERN "*.h")
