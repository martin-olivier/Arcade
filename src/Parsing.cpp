#include "Parsing.hpp"
#include "Exception.hpp"
#include "DyLib.hpp"
#include <filesystem>
#include <iostream>
#include <algorithm>

#include "IDisplay.hpp"
#include "IGame.hpp"

Arcade::Parsing::Parsing(int ac, char **av) : m_GameLibsIterator(0), m_GraphLibsIterator(0), m_name("unknow")
{
    if (ac != 2)
        throw Arcade::exception("USAGE : " + std::string(av[0]) + " lib.so");

    for (const auto &entry : std::filesystem::directory_iterator("./lib")) {
        for (auto graphLib : GRAPHICS_LIBRARIES) {
            if (entry.path() == graphLib)
                m_GraphLibs.emplace_back(graphLib);
        }
        for (auto gameLib : GAMES_LIBRARIES) {
            if (entry.path() == gameLib)
                m_GameLibs.emplace_back(gameLib);
        }
    }
    if (std::find(m_GraphLibs.begin(), m_GraphLibs.end(), av[1]) == std::end(m_GraphLibs))
        throw Arcade::exception("Cannot load the dynamic library : " + std::string(av[1]));
    if (m_GameLibs.empty())
        throw Arcade::exception("No Game Libs found or no Game Libs could be loaded... Exiting");

    std::cout << "\nGAMES LIBRARIES :\n";
    for (const auto &gameLib : m_GameLibs)
        std::cout << gameLib << std::endl;
    std::cout << "\nGRAPHICS LIBRARIES :\n";
    for (const auto &graphLib : m_GraphLibs)
        std::cout << graphLib << std::endl;
    std::cout << "\nEnter your name :" << std::endl;
    std::getline(std::cin, m_name);
    m_GraphLibsIterator = std::distance(m_GraphLibs.begin(), std::find(m_GraphLibs.begin(), m_GraphLibs.end(), av[1]));
    try {DyLib<Arcade::IDisplay> tmp("./lib/arcade_sdl2.so");}
    catch (...) {}
}