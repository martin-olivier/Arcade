#include "NCURSES.hpp"
#include <ncurses.h>
#include <map>
#include <cmath>

extern "C" Arcade::NCURSES *Arcade::entry_point()
{
    return new Arcade::NCURSES;
}

std::map<Arcade::Color, unsigned int> colormap {
        {Arcade::Color::BLACK, 0},
        {Arcade::Color::RED, 1},
        {Arcade::Color::GREEN, 2},
        {Arcade::Color::YELLOW, 3},
        {Arcade::Color::BLUE, 4},
        {Arcade::Color::MAGENTA, 5},
        {Arcade::Color::CYAN, 6},
        {Arcade::Color::WHITE, 7},
};

Arcade::NCURSES::NCURSES()
{
    initscr();
    noecho();
    keypad(stdscr, TRUE);
    nodelay(stdscr, true);
    curs_set(FALSE);

    start_color();
    init_pair(0, COLOR_BLACK, 0);
    init_pair(1, COLOR_RED, 0);
    init_pair(2, COLOR_GREEN, 0);
    init_pair(3, COLOR_YELLOW, 0);
    init_pair(4, COLOR_BLUE, 0);
    init_pair(5, COLOR_MAGENTA, 0);
    init_pair(6, COLOR_CYAN, 0);
    init_pair(7, COLOR_WHITE, 0);
    init_pair(8, 14, 0);
    init_pair(9, 40, 0);
}

Arcade::NCURSES::~NCURSES()
{
    endwin();
}

Arcade::Input Arcade::NCURSES::event()
{
    Arcade::Input input = NIL;
    int c = getch();

    while (c != ERR) {
        if (c == KEY_UP)
            input = UP;
        else if (c == KEY_DOWN)
            input = DOWN;
        else if (c == KEY_LEFT)
            input = LEFT;
        else if (c == KEY_RIGHT)
            input = RIGHT;
        else if (c == 10)
            input = ENTER;
        else if (c == 32)
            input = SPACE;
        else if (c == 27)
            input = ESCAPE;
        else if (c == 114)
            input = RESET;
        else if (c == 109)
            input = MENU;
        else if (c == 110)
            input = N;
        else if (c == 112)
            input = P;
        else if (c == 105)
            input = I;
        else if (c == 111)
            input = O;
        c = getch();
    }
    return input;
}

void Arcade::NCURSES::refresh()
{
    wrefresh(stdscr);
}

void Arcade::NCURSES::clear()
{
    wclear(stdscr);
}

void Arcade::NCURSES::draw(std::shared_ptr<Arcade::IObject> object)
{
    if (dynamic_cast<Arcade::DynamicTile*>(object.get()) != nullptr)
        drawTile(dynamic_cast<Arcade::DynamicTile*>(object.get())->getActualTile());
    else if (dynamic_cast<Arcade::Tile*>(object.get()) != nullptr)
        drawTile(dynamic_cast<Arcade::Tile*>(object.get()));
    else if (dynamic_cast<Arcade::Text*>(object.get()) != nullptr)
        drawText(dynamic_cast<Arcade::Text*>(object.get()));
}

void Arcade::NCURSES::drawTile(Arcade::Tile *tile)
{
    char tmp[2];
    tmp[0] = static_cast<char>(tile->getSymbol());
    tmp[1] = '\0';

    if (has_colors() == FALSE) {
        mvprintw(round(tile->getPosition().second), round(tile->getPosition().first), tmp);
        return;
    }
    attron(COLOR_PAIR(colormap[tile->getColor()]));
    mvprintw(round(tile->getPosition().second), round(tile->getPosition().first), tmp);
    attroff(COLOR_PAIR(tile->getColor()));
}

void Arcade::NCURSES::drawText(Arcade::Text *text)
{
    if (has_colors() == FALSE) {
        mvprintw(round(text->getPosition().second), round(text->getPosition().first), text->getText().c_str());
        return;
    }
    attron(COLOR_PAIR(colormap[text->getColor()]));
    mvprintw(round(text->getPosition().second), round(text->getPosition().first), text->getText().c_str());
    attroff(COLOR_PAIR(text->getColor()));
}