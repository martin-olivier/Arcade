cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

set(BUILD_SHARED_LIBS ON)

add_library(nibbler SHARED
        src/Snake.cpp
        ../src/CoordinatesCompute.cpp)

set_target_properties(nibbler PROPERTIES PREFIX "arcade_")

target_include_directories(nibbler PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/Game/Snake/include
        ${PROJECT_SOURCE_DIR}/lib/Game/include
        ${PROJECT_SOURCE_DIR}/include)

add_custom_command(TARGET nibbler POST_BUILD
        COMMAND cp arcade_nibbler.so ${PROJECT_SOURCE_DIR}/lib)
