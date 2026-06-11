cmake_minimum_required(VERSION 3.20)

project(cpp-vampire LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(SDL3 CONFIG REQUIRED)
find_package(SDL3_image CONFIG REQUIRED)

add_executable(cpp-vampire
    src/App.cpp
    src/main.cpp
)

target_link_libraries(cpp-vampire PRIVATE SDL3::SDL3 SDL3_image::SDL3_image)

add_custom_command(TARGET cpp-vampire POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        $<TARGET_FILE_DIR:cpp-vampire>/assets
)
