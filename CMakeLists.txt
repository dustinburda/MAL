cmake_minimum_required(VERSION 3.15)

project(MAL)

set(CMAKE_CXX_STANDARD 23)

add_executable(MAL step1_read_print.cpp reader.cpp types.cpp printer.cpp)