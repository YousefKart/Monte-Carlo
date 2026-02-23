cmake_minimum_required(VERSION 3.20)
project(MonteCarlo LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(MonteCarlo
    src/main.cpp
)

if (MSVC)
    target_compile_options(MonteCarlo PRIVATE /W4)
else()
    target_compile_options(MonteCarlo PRIVATE -Wall -Wextra -Wpedantic)
endif()
