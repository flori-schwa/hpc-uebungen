cmake_minimum_required(VERSION 3.20)
project(matrixadd C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_FLAGS "-O3 -mavx -lm")
set(CMAKE_C_FLAGS_DEBUG "-Og -mavx -lm")

include_directories(.)

add_executable(matrixadd matrixadd-simd.c)
