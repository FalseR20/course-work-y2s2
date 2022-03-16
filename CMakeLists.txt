# Project settings
cmake_minimum_required(VERSION 3.19)
project(course-work-y2s2)

# Version of C++
set(CMAKE_CXX_STANDARD 17)

# Libraries connection
find_package(MPI REQUIRED)
include_directories("C:/libjpeg-turbo-gcc/include")

# Adding executable files
add_executable(course-work-y2s2 main.cpp)

# Add library for code completion and compiling
target_link_libraries(course-work-y2s2 PUBLIC MPI::MPI_CXX )
