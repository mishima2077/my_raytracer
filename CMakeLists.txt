cmake_minimum_required(VERSION 3.16)
project(my_raytracer LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

find_package(SDL3 REQUIRED)

add_library(glad STATIC third_party/glad/src/glad.c)
target_include_directories(glad PUBLIC third_party/glad/include)
target_link_libraries(glad PUBLIC ${CMAKE_DL_LIBS})

add_executable(raytracer 
    src/main.cpp
    src/camera.cpp
    src/color.cpp
    src/sphere.cpp
    src/material.cpp
    src/hittable_list.cpp
    src/window.cpp
)

target_include_directories(raytracer PRIVATE include)

target_compile_options(raytracer PRIVATE
    -Wall -Wextra -Wpedantic -march=native
)

target_link_libraries(raytracer PRIVATE
    SDL3::SDL3
    glad
)
