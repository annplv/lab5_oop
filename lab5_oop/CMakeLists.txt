cmake_minimum_required(VERSION 3.10)
project(Lab05)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(include)

add_executable(lab5_oop_main
    src/main.cpp
)

add_subdirectory(tests)
