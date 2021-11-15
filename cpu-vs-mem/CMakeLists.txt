cmake_minimum_required(VERSION 3.20)
project(cpuvsmem C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-fopenmp -mavx -lm")
set(CMAKE_C_FLAGS_RELEASE "-O3")

add_executable(cpuvsmem cpu-vs-mem.c)
