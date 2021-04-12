#pragma once

#include "Parsing.hpp"
#include "IGame.hpp"
#include "IDisplay.hpp"
#include "DyLib.hpp"
#include "HighScore.hpp"

namespace Arcade
{
    class Core : public Parsing
    {
    private:
        DyLib<Arcade::IGame> m_game;
        DyLib<Arcade::IDisplay> m_display;
        HighScore m_score {};
    public:
        /**
         *  The Core class takes the program arguments as parameters. 
         *  It is the bridge between graphic libraries and games
         *
         *  @param ac arguments counter
         *  @param av arguments value
         */
        Core(int ac, char **av);
        ~Core() = default;
        /**
         *  Loads the previous graphical library available
         */
        void prevDisplay();
        /**
         *  Loads the next graphical library available
         */
        void nextDisplay();
        /**
         *  Loads the previous game library available
         */
        void prevGame();
        /**
         *  Loads the next game library available
         */
        void nextGame();
        /**
         *  Loads the menu
         *
         *  @return false if the user exited, true otherwise
         */
        bool menu();
    };
}