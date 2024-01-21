cmake_minimum_required(VERSION 3.16)
project(mesh_stuff LANGUAGES CXX)

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

set(CMAKE_CXX_STANDARD 17)

file(GLOB cpp_sources ${PROJECT_SOURCE_DIR}/src/*.cpp)
add_library(mesh_stuff ${cpp_sources})
target_include_directories(mesh_stuff PUBLIC ${PROJECT_SOURCE_DIR}/inc)

find_package(ZLIB 1.2.3) # defines imported target ZLIB::ZLIB
if (ZLIB_FOUND) 
  target_link_libraries(mesh_stuff PUBLIC ZLIB::ZLIB)
else()
  include(FetchContent)
  FetchContent_Declare(
    zlib
    URL https://github.com/madler/zlib/releases/download/v1.3/zlib-1.3.tar.gz
  )
  FetchContent_MakeAvailable(zlib)
  target_link_libraries(mesh_stuff PUBLIC zlib)
endif()

include(FetchContent)
include(ExternalProject)
include(cmake/options.cmake)

if (MESH_ENABLE_TESTING)
  add_subdirectory(tests)
endif()
