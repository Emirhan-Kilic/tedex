cmake_minimum_required(VERSION 3.28)
project(TexEd C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)


add_executable(TexEd main.c)
target_compile_options(TexEd PRIVATE -Wall -Wextra -pedantic)
