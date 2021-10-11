cmake_minimum_required(VERSION 3.20)
project(matrixadd C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_CXX_FLAGS_DEBUG "-Og")

include_directories(.)

add_executable(matrixadd matrixadd.c)
