cmake_minimum_required(VERSION 3.20)
project(MonteCarlo LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# C++ Standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Executable
add_executable(MonteCarlo
    src/main.cpp
    src/engine/functions.cpp
)

# Project Include Directory (your own headers)
target_include_directories(MonteCarlo PRIVATE
    include
)

# GLAD
add_library(glad
    external/glad/src/glad.c
)
target_include_directories(glad PUBLIC
    external/glad/include
)

# GLFW (build from source)
set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE)

add_subdirectory(external/glfw-3.4)

# OpenGL
find_package(OpenGL REQUIRED)

# Link Libraries
target_link_libraries(MonteCarlo PRIVATE
    glad
    glfw
    OpenGL::GL
)

# Compiler Warnings
if (MSVC)
    target_compile_options(MonteCarlo PRIVATE /W4)
else()
    target_compile_options(MonteCarlo PRIVATE -Wall -Wextra -Wpedantic)
endif()
