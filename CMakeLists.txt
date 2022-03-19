# Project settings
cmake_minimum_required(VERSION 3.19)
project(course_work_y2s2)

# Version of C++
set(CMAKE_CXX_STANDARD 14)

# MPI connection
find_package(MPI REQUIRED)

# TurboJPEG connection
set(CMAKE_PREFIX_PATH C:\\libjpeg-turbo-gcc)
find_package(PkgConfig REQUIRED)
pkg_search_module(TURBOJPEG REQUIRED libturbojpeg)
link_directories(${TURBOJPEG_LIBDIR})

# Adding executable files
add_executable(course_work_y2s2 main.cpp Image.h Image.cpp)

# Add libraries for code completion and compiling
target_include_directories(course_work_y2s2 PUBLIC ${TURBOJPEG_INCLUDE_DIRS})
target_link_libraries(course_work_y2s2 PUBLIC ${TURBOJPEG_LIBRARIES})
target_link_libraries(course_work_y2s2 PUBLIC MPI::MPI_CXX )
