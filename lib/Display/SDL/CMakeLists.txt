cmake_minimum_required(VERSION 3.17)

set(CMAKE_CXX_STANDARD 20)

add_library(sdl2 SHARED
        src/SDL.cpp)

set_target_properties(sdl2 PROPERTIES PREFIX "arcade_")

target_link_libraries(sdl2 PRIVATE
        SDL2
        SDL2_image
        SDL2_mixer
        SDL2_ttf)

target_include_directories(sdl2 PRIVATE
        ${PROJECT_SOURCE_DIR}/lib/Display/SDL/include
        ${PROJECT_SOURCE_DIR}/lib/Display/include
        ${PROJECT_SOURCE_DIR}/include)

add_custom_command(TARGET sdl2 POST_BUILD
        COMMAND cp arcade_sdl2.so ${PROJECT_SOURCE_DIR}/lib)
