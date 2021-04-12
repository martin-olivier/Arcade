#pragma once

#include "IDisplay.hpp"
#include <iostream>

namespace Arcade
{
    class NCURSES : public IDisplay
    {
    public:
        NCURSES();
        ~NCURSES();
        Arcade::Input event();
        void clear();
        void refresh();
        void draw(std::shared_ptr<Arcade::IObject> object);
        void drawTile(Arcade::Tile *tile);
        void drawText(Arcade::Text *text);
        void playSound(Arcade::Sound *) {};
    };
    extern "C" NCURSES *entry_point();
}