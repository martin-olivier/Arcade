#include <filesystem>
#include <map>
#include <vector>
#include "Menu.hpp"
#include "Exception.hpp"
#include "Parsing.hpp"

extern "C" Arcade::Menu *Arcade::entry_point()
{
    return new Arcade::Menu;
}

std::map<std::string, std::string> m_menu_displayer_map = {
    {"./lib/arcade_pacman.so", "assets/Menu/pacman_affiche.png"},
    {"./lib/arcade_nibbler.so", "assets/Menu/nibbler_affiche.png"},
    {"./lib/arcade_qix.so", "assets/Menu/other_affiche.png"},
    {"./lib/arcade_centipede.so", "assets/Menu/other_affiche.png"},
    {"./lib/arcade_solarfox.so", "assets/Menu/other_affiche.png"},
};

Arcade::Menu::Menu() : m_score(0), m_iterator(0)
{
    Arcade::setTileSize(30);

    for (const auto &entry : std::filesystem::directory_iterator("./lib")) {
        for (auto gameLib : GAMES_LIBRARIES) {
            if (entry.path() == gameLib) {
                m_GameLibs.emplace_back(gameLib);
                std::string tmp(gameLib);
                auto other = tmp.substr(13);
                other.pop_back();
                other.pop_back();
                other.pop_back();
                m_text_list.push_back(std::make_shared<Arcade::Text>("> " + other, WHITE, 10, 8));
            }
        }
    }

    m_menu_title = std::make_shared<Arcade::Text>("Arcade - Menu", WHITE, 16, 1);
    m_arcade = std::make_shared<Arcade::Tile>("assets/Menu/arcade.png", 0, WHITE, 3, 3);
    m_poster = std::make_shared<Arcade::Tile>(m_menu_displayer_map[m_GameLibs[0]], ' ', WHITE, 13, 11);

    m_menu_enter = std::make_shared<Arcade::Text>("Select a game and press ENTER !", WHITE, 12, 30);
    m_menu_exit = std::make_shared<Arcade::Text>("Press ECHAP to exit Arcade", WHITE, 13, 31);

    m_sou = std::make_shared<Arcade::Sound>("assets/Menu/move_menu.wav");
    m_high_scores.load(m_GameLibs[0]);
}

void Arcade::Menu::reset()
{
}

std::vector<std::shared_ptr<Arcade::IObject>> Arcade::Menu::loop(Arcade::Input ev)
{
    if (ev != Input::NIL)
        m_ev = ev;
    if (clock::now() - m_clock < m_timestep)
        return (std::vector<std::shared_ptr<Arcade::IObject>>{});
    std::vector<std::shared_ptr<Arcade::IObject>> buf{};
    if (m_ev == LEFT or m_ev == UP) {
        m_score--;
        if (m_score < 0)
            m_score = static_cast<int>(m_text_list.size()) - 1;
        m_poster->setPath(m_menu_displayer_map[m_GameLibs[m_score]]);
        m_high_scores.load(m_GameLibs[m_score]);
        buf.push_back(m_sou);
        m_ev = Input::NIL;
    }
    else if (m_ev == RIGHT or m_ev == DOWN) {
        m_score++;
        if (m_score >= static_cast<int>(m_text_list.size()))
            m_score = 0;
        m_poster->setPath(m_menu_displayer_map[m_GameLibs[m_score]]);
        m_high_scores.load(m_GameLibs[m_score]);
        buf.push_back(m_sou);
        m_ev = Input::NIL;
    }
    buf.push_back(std::make_shared<Arcade::Text>("HIGHSCORE :", WHITE, 45, 10));
    std::vector<std::string> tmp = m_high_scores.getContent();
    m_clock = clock::now();
    for (size_t i = 0; i < 5 and i < tmp.size(); i++)
        buf.push_back(std::make_shared<Arcade::Text>(tmp[i], WHITE, 45, (i + 2) + 10));
    buf.push_back(m_text_list[m_score]);
    buf.push_back(m_arcade);
    buf.push_back(m_poster);
    buf.push_back(m_menu_title);
    buf.push_back(m_menu_enter);
    buf.push_back(m_menu_exit);
    return buf;
}
