cmake_minimum_required(VERSION 3.15)

project(hw3d_demo LANGUAGES CXX)

# Gather demo sources (C/C++ files). If none exist, do nothing.
file(GLOB DEMO_SOURCES
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cc"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
  "${CMAKE_CURRENT_SOURCE_DIR}/*.c"
)

if(NOT DEMO_SOURCES)
  message(STATUS "No demo sources found in ${CMAKE_CURRENT_SOURCE_DIR}; skipping demo target.")
  return()
endif()

# Demo executable that links the hw3d static library from the sibling hw3d directory
add_executable(hw3d_demo WIN32 ${DEMO_SOURCES})

target_include_directories(hw3d_demo PRIVATE
  ${CMAKE_CURRENT_SOURCE_DIR}
)

# compile demo resources (icon) into the executable if present
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/resource.rc")
  target_sources(hw3d_demo PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/resource.rc")
endif()

if(MSVC)
  target_compile_options(hw3d_demo PRIVATE /W4 /permissive-)
else()
  target_compile_options(hw3d_demo PRIVATE -Wall -Wextra -Wpedantic)
endif()

# Link against the static hw3d library produced by the hw3d subdirectory
target_link_libraries(hw3d_demo PRIVATE hw3d_static user32 gdi32)

# Respect the MBCS option from the parent project when present
if(USE_MULTI_BYTE_CHARSET)
  target_compile_definitions(hw3d_demo PRIVATE _MBCS)
endif()

# Install demo executable
install(TARGETS hw3d_demo
        RUNTIME DESTINATION bin)
