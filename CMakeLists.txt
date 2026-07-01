cmake_minimum_required(VERSION 3.20)
project(canary)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(canary
    src/main.cpp
)

target_include_directories(canary PRIVATE include)
