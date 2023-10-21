cmake_minimum_required(VERSION 3.16)
project(mesh_io LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)

file(GLOB cpp_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(mesh_io ${cpp_sources})
target_include_directories(mesh_io PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(mesh_io PUBLIC zstr::zstr)

include(FetchContent)
include(ExternalProject)
include(cmake/options.cmake)
include(cmake/zstr.cmake) # zlib wrapper library

if (ENABLE_TESTING)
  add_subdirectory(tests)
endif()
