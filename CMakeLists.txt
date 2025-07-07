cmake_minimum_required(VERSION 3.27)

project(MAL)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Werror -Wextra)

add_executable(MAL src/step2_eval.cpp src/reader.cpp src/types.cpp src/printer.cpp)