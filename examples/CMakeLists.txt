cmake_minimum_required(VERSION 3.11)

project(examples DESCRIPTION "allegro++ examples")

add_executable(example0 example0.cpp)

include_directories(example0 PUBLIC
   ${CMAKE_SOURCE_DIR}/include/
)

find_library(ALLEGROPP
   NAMES allegro++ 
   HINTS ${CMAKE_BUILD_DIR})

target_link_libraries(example0 PUBLIC
   allegro++
   allegro
   allegro_primitives
   allegro_memfile
   allegro_ttf
   allegro_image
   allegro_dialog
   allegro_font
   allegro_color)