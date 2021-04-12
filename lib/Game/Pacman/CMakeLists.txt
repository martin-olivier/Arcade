cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS ON)

add_library(pacman SHARED
        src/Pacman.cpp
        ../src/CoordinatesCompute.cpp)

set_target_properties(pacman PROPERTIES PREFIX "arcade_")

target_include_directories(pacman PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/Game/Pacman/include
        ${PROJECT_SOURCE_DIR}/lib/Game/include
        ${PROJECT_SOURCE_DIR}/include)

add_custom_command(TARGET pacman POST_BUILD
        COMMAND cp arcade_pacman.so ${PROJECT_SOURCE_DIR}/lib)
