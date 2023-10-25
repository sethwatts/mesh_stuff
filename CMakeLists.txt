cmake_minimum_required(VERSION 3.16)
project(mesh_stuff LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)

find_package(ZLIB 1.2.3 REQUIRED) # defines imported target ZLIB::ZLIB

file(GLOB cpp_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(mesh_stuff ${cpp_sources})
target_include_directories(mesh_stuff PUBLIC ${PROJECT_SOURCE_DIR}/inc)
target_link_libraries(mesh_stuff PUBLIC ZLIB::ZLIB)

include(FetchContent)
include(ExternalProject)
include(cmake/options.cmake)

if (MESH_ENABLE_TESTING)
  add_subdirectory(tests)
endif()
