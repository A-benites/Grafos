cmake_minimum_required(VERSION 4.2)
project(ayudantia8)

set(CMAKE_CXX_STANDARD 20)

add_executable(ayudantia8
        main.cpp
        struct/Grafo.cpp
        include/struct/ArregloDinamico.h)

target_include_directories(ayudantia8 PRIVATE include)
