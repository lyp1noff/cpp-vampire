cmake_minimum_required(VERSION 3.20)

project(cpp-vampire LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)

add_executable(cpp-vampire
    src/main.cpp
)

target_link_libraries(cpp-vampire PRIVATE SDL3::SDL3)