cmake_minimum_required(VERSION 3.20)
project(tiling C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-fopenmp")

add_executable(tiling openmp-tiling.c)