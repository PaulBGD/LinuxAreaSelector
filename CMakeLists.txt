cmake_minimum_required(VERSION 3.5k)
project(LinuxAreaSelector)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})
link_directories(${GTK3_LIBRARY_DIRS})
add_definitions(${GTK3_CFLAGS_OTHER})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp)
add_executable(LinuxAreaSelector ${SOURCE_FILES})
target_link_libraries(LinuxAreaSelector ${GTK3_LIBRARIES})