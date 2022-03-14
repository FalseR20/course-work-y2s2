cmake_minimum_required(VERSION 3.19)
project(course_work_y2s2)

set(CMAKE_CXX_STANDARD 17)

# Add MPI Package to Project
find_package(MPI REQUIRED)

add_executable(course_work_y2s2 main.cpp)

# Add MPI library for code completion and compiling
target_link_libraries(course_work_y2s2 PUBLIC MPI::MPI_CXX)
