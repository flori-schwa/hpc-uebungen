cmake_minimum_required(VERSION 3.20)
project(password-crack C)

set(CMAKE_C_STANDARD 11)

set(CMAKE_C_FLAGS "-fopenmp")

add_executable(password-crack password-crack.c sha-256.h sha-256.c)