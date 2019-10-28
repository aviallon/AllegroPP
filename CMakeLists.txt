cmake_minimum_required(VERSION 3.9)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIBNAME "allegro++")
set(LIBVERSION 1.0.0)

project(${LIBNAME} VERSION ${LIBVERSION} DESCRIPTION "allegro but for C++")

add_subdirectory(examples)

include(GNUInstallDirs)

set(InstallDir ${PROJECT_SOURCE_DIR})
configure_file(
   ${PROJECT_SOURCE_DIR}/src/allegro++/primitives.cpp.in
   ${PROJECT_SOURCE_DIR}/src/allegro++/primitives.cpp)

file(GLOB SRC_FILES
    ${PROJECT_SOURCE_DIR}/src/allegro++/*.cpp
    ${PROJECT_SOURCE_DIR}/include/allegro++/*.h
    )
set(UNSUPPORTED_OS true)

IF(CMAKE_SYSTEM_NAME STREQUAL "Linux")
    set(UNSUPPORTED_OS false)
#    file(GLOB LINUX_SRC_FILES
#         ${PROJECT_SOURCE_DIR}/src/allegro_flare/platform/linux/*.cpp)
#    list (APPEND SRC_FILES ${LINUX_SRC_FILES})
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Linux")

IF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")
    set(UNSUPPORTED_OS false)
#    file(GLOB MACOS_SRC_FILES
#        ${PROJECT_SOURCE_DIR}/src/allegro_flare/platform/mac_os/*.cpp)
#    list (APPEND SRC_FILES ${MACOS_SRC_FILES})
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Darwin")

IF(CMAKE_SYSTEM_NAME STREQUAL "Windows")
    set(UNSUPPORTED_OS false)
#    file(GLOB WIN_SRC_FILES
#        ${PROJECT_SOURCE_DIR}/src/allegro_flare/platform/win/*.cpp)
#    list (APPEND SRC_FILES ${WIN_SRC_FILES})
ENDIF(CMAKE_SYSTEM_NAME STREQUAL "Windows")

IF(UNSUPPORTED_OS)
#   file(GLOB UNSUPPORTED_SRC_FILES
#        ${PROJECT_SOURCE_DIR}/src/allegro_flare/platform/generic/*.cpp)
#    list (APPEND SRC_FILES ${UNSUPPORTED_SRC_FILES})
ENDIF(UNSUPPORTED_OS)

add_library(${LIBNAME} SHARED ${SRC_FILES})

set_target_properties(${LIBNAME} PROPERTIES
    VERSION ${LIBVERSION}
    SOVERSION 1
    PUBLIC_HEADER include/allegro++/allegro.h)
    
target_include_directories(${LIBNAME} PRIVATE include src)

find_library(ALLEGRO_LIBS NAMES
   allegro
   allegro_color
   allegro_dialog
   allegro_font
   allegro_image
   allegro_memfile
   allegro_primitives
   allegro_ttf
   armadillo
   stdc++fs)

target_link_libraries(${LIBNAME} allegro)

install(TARGETS ${LIBNAME}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    PUBLIC_HEADER DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
