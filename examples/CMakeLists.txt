cmake_minimum_required(VERSION 3.11)

project(examples DESCRIPTION "allegro++ examples")

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_library(ALLEGROPP
   NAMES allegro++ 
   HINTS ${CMAKE_BUILD_DIR})

link_libraries(
   allegro++
   allegro
   allegro_primitives
   allegro_memfile
   allegro_ttf
   allegro_image
   allegro_dialog
   allegro_font
   allegro_color
   armadillo
   GL
   GLU
   stdc++fs)
   
include_directories(
   ${CMAKE_SOURCE_DIR}/include/
)

add_executable(example0 example0.cpp)

add_executable(example1 example1.cpp)

add_executable(example2 example2.cpp)

add_executable(example3 example3.cpp)