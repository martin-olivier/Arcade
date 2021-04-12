cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS ON)

add_library(menu SHARED
        src/Menu.cpp
        ../src/HighScore.cpp)

set_target_properties(menu PROPERTIES PREFIX "arcade_")

target_include_directories(menu PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/Game/Menu/include
        ${PROJECT_SOURCE_DIR}/lib/Game/include
        ${PROJECT_SOURCE_DIR}/include)

add_custom_command(TARGET menu POST_BUILD
        COMMAND cp arcade_menu.so ${PROJECT_SOURCE_DIR}/lib)
