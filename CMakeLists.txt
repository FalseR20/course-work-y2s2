## Project settings
#cmake_minimum_required(VERSION 3.19)
#project(course-work-y2s2)
#
## Version of C++
#set(CMAKE_CXX_STANDARD 17)
#
## Libraries connection
#find_package(MPI REQUIRED)
##include(FindPkgConfig)
##pkg_check_modules()
#include_directories("C:/libjpeg-turbo-gcc/include")
#
## Adding executable files
#add_executable(course-work-y2s2 main.cpp)
#
## Add library for code completion and compiling
#target_link_libraries(course-work-y2s2 PUBLIC MPI::MPI_CXX )

cmake_minimum_required(VERSION 3.19)
project(New_Kursuch)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_PREFIX_PATH C:\\libjpeg-turbo-gcc)

find_package(PkgConfig REQUIRED)
find_package(MPI REQUIRED)

pkg_search_module(TURBOJPEG REQUIRED libturbojpeg)

link_directories(${TURBOJPEG_LIBDIR})
add_executable(New_Kursuch main.cpp)

target_include_directories(New_Kursuch PUBLIC ${TURBOJPEG_INCLUDE_DIRS})
target_link_libraries(New_Kursuch PUBLIC ${TURBOJPEG_LIBRARIES})
target_link_libraries(New_Kursuch PUBLIC MPI::MPI_CXX )

#cmake_minimum_required(VERSION 3.19)
#project(cursa4_2_2)
#set(CMAKE_CXX_STANDARD 14)
#set(CMAKE_PREFIX_PATH C:\\libjpeg-turbo-gcc)
#
#find_package(PkgConfig REQUIRED)
#find_package(MPI REQUIRED)
#
#pkg_search_module(TURBOJPEG REQUIRED libturbojpeg)
#
#link_directories(${TURBOJPEG_LIBDIR})
#add_executable(cursa4_2_2 main.cpp)
#
#target_include_directories(cursa4_2_2 PUBLIC ${MPI_CXX_INCLUDE_PATH} ${TURBOJPEG_INCLUDE_DIRS})
#target_link_libraries(cursa4_2_2 ${MPI_CXX_LIBRARIES} ${TURBOJPEG_LIBRARIES})