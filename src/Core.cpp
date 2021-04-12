#include "Core.hpp"
#include <thread>

using namespace std::chrono_literals;

bool Arcade::Core::menu()
{
    m_game.loadLib("./lib/arcade_menu.so");
    while (true) {
        auto menu_input = m_display.get()->event();
        if (menu_input == ESCAPE)
            return false;
        else if (menu_input == ENTER) {
            auto index = m_game.get()->getScore();
            m_game.loadLib(m_GameLibs[index]);
            m_score.load(m_GameLibs[index]);
            return true;
        }
        else if (menu_input == P)
            prevDisplay();
        else if (menu_input == N)
            nextDisplay();
        else {
            auto buffer = m_game.get()->loop(menu_input);
            if (!buffer.empty()) {
                m_display.get()->clear();
                for (auto &tile : buffer)
                    m_display.get()->draw(tile);
                m_display.get()->refresh();
            }
        }
    }
}

Arcade::Core::Core(int ac, char **av) : Arcade::Parsing(ac, av), m_game(), m_display()
{
    m_display.loadLib(m_GraphLibs[m_GraphLibsIterator]);
    if (menu() == false)
        return;

    while (true) {
        auto input = m_display.get()->event();
        if (input == ESCAPE) {
            m_score.setNewScore(std::to_string(m_game.get()->getScore()) + " " + m_name);
            m_score.writeNewScore();
            return;
        }
        else if (input == MENU) {
            m_score.setNewScore(std::to_string(m_game.get()->getScore()) + " " + m_name);
            m_score.writeNewScore();
            if (menu() == false)
                return;
        }
        else if (input == P)
            prevDisplay();
        else if (input == N)
            nextDisplay();
        else if (input == I)
            prevGame();
        else if (input == O)
            nextGame();
        else if (input == RESET)
            m_game.get()->reset();
        else {
            auto buffer = m_game.get()->loop(input);
            if (!buffer.empty()) {
                m_display.get()->clear();
                for (auto &tile : buffer)
                    m_display.get()->draw(tile);
                m_display.get()->refresh();
            }
        }
        std::this_thread::sleep_for(6000us);
    }
}

void Arcade::Core::prevDisplay()
{
    m_GraphLibsIterator--;
    if (m_GraphLibsIterator < 0)
        m_GraphLibsIterator = static_cast<int>(m_GraphLibs.size()) - 1;
    m_display.loadLib(m_GraphLibs[m_GraphLibsIterator]);
}

void Arcade::Core::nextDisplay()
{
    m_GraphLibsIterator++;
    if (m_GraphLibsIterator >= static_cast<int>(m_GraphLibs.size()))
        m_GraphLibsIterator = 0;
    m_display.loadLib(m_GraphLibs[m_GraphLibsIterator]);
}

void Arcade::Core::prevGame()
{
    m_GameLibsIterator--;
    if (m_GameLibsIterator < 0)
        m_GameLibsIterator = static_cast<int>(m_GameLibs.size()) - 1;
    m_game.loadLib(m_GameLibs[m_GameLibsIterator]);
    m_score.load(m_GameLibs[m_GameLibsIterator]);
}

void Arcade::Core::nextGame()
{
    m_GameLibsIterator++;
    if (m_GameLibsIterator >= static_cast<int>(m_GameLibs.size()))
        m_GameLibsIterator = 0;
    m_game.loadLib(m_GameLibs[m_GameLibsIterator]);
    m_score.load(m_GameLibs[m_GameLibsIterator]);
}
