cmake_minimum_required(VERSION 3.28.3)
project(Project2_DSA)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)

add_executable(Project2_DSA
        src/heap_sort.cpp
        src/heap_sort.h
        src/main.cpp)
