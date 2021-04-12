#pragma once

#include <string>
#include <vector>
#include <string_view>

namespace Arcade
{
    /**
     *  List of all possible graphical libraries
     */
    constexpr std::string_view GRAPHICS_LIBRARIES[] = {
        "./lib/arcade_ncurses.so",
        "./lib/arcade_sdl2.so",
        "./lib/arcade_ndk++.so",
        "./lib/arcade_aalib.so",
        "./lib/arcade_libcaca.so",
        "./lib/arcade_allegro5.so",
        "./lib/arcade_xlib.so",
        "./lib/arcade_gtk+.so",
        "./lib/arcade_sfml.so",
        "./lib/arcade_irrlicht.so",
        "./lib/arcade_opengl.so",
        "./lib/arcade_vulkan.so",
        "./lib/arcade_qt5.so",
    };

    /**
     *  List of all possible games libraries
     */
    constexpr std::string_view GAMES_LIBRARIES[] = {
        "./lib/arcade_nibbler.so",
        "./lib/arcade_pacman.so",
        "./lib/arcade_qix.so",
        "./lib/arcade_centipede.so",
        "./lib/arcade_solarfox.so",
    };
    class Parsing
    {
    protected:
        std::vector<std::string> m_GameLibs {};
        int m_GameLibsIterator;
        std::vector<std::string> m_GraphLibs {};
        int m_GraphLibsIterator;
        std::string m_name;

    public:
        Parsing(int ac, char **av);
        ~Parsing() = default;
    };
}