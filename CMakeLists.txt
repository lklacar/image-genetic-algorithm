cmake_minimum_required(VERSION 3.13)
project(image_genetic_algorithm C)

set(CMAKE_C_STANDARD 99)

find_package(PNG REQUIRED)
include_directories(${PNG_INCLUDE_DIR})

add_executable(image_genetic_algorithm src/main.c src/image/genalg_image.c src/image/genalg_image.h)

target_link_libraries(image_genetic_algorithm ${PNG_LIBRARY})