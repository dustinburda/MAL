cmake_minimum_required(VERSION 3.27)

project(MAL)

set(CMAKE_CXX_STANDARD 23)
add_compile_options(-Wall -Werror -Wextra)

add_executable(MAL src/step3_env.cpp src/reader.cpp src/types.cpp src/printer.cpp src/environment.cpp)