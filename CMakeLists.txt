cmake_minimum_required(VERSION 3.20)
project(spsc_benchmark)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(ENABLE_TSAN "Enable ThreadSanitizer" OFF)
if(ENABLE_TSAN)
    add_compile_options(-fsanitize=thread -g)
    add_link_options(-fsanitize=thread)
endif()

find_package(benchmark REQUIRED)

add_executable(spsc_ring_buffer
    src/main.cpp
)
target_include_directories(spsc_ring_buffer PRIVATE include)

add_executable(spsc_benchmark
    src/benchmark.cpp
)
target_include_directories(spsc_benchmark PRIVATE include)
target_link_libraries(spsc_benchmark PRIVATE benchmark::benchmark)

