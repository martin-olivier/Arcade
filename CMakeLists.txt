cmake_minimum_required(VERSION 3.17)
project(arcade)

set(CMAKE_CXX_STANDARD 20)

add_compile_options(-Wall -Wextra -Weffc++)

set_property(DIRECTORY APPEND PROPERTY ADDITIONAL_MAKE_CLEAN_FILES
        ${PROJECT_SOURCE_DIR}/arcade
        ${PROJECT_SOURCE_DIR}/lib/arcade_ncurses.so
        ${PROJECT_SOURCE_DIR}/lib/arcade_sdl2.so
        ${PROJECT_SOURCE_DIR}/lib/arcade_sfml.so
        ${PROJECT_SOURCE_DIR}/lib/arcade_nibbler.so
        ${PROJECT_SOURCE_DIR}/lib/arcade_pacman.so
        ${PROJECT_SOURCE_DIR}/lib/arcade_menu.so)

add_executable(arcade
        src/main.cpp
        src/Core.cpp
        src/Parsing.cpp
        lib/Game/src/HighScore.cpp)

target_link_libraries(arcade PUBLIC dl)

target_include_directories(arcade PUBLIC
        include
        lib/Display/include
        lib/Game/include)

add_subdirectory(lib/Display/Ncurses)
add_subdirectory(lib/Display/SDL)
add_subdirectory(lib/Display/SFML)

add_subdirectory(lib/Game/Pacman)
add_subdirectory(lib/Game/Snake)
add_subdirectory(lib/Game/Menu)

add_dependencies(arcade ncurses sdl2 sfml pacman nibbler menu)

add_custom_command(TARGET arcade POST_BUILD
        COMMAND cp arcade ${PROJECT_SOURCE_DIR})