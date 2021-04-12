#pragma once

#include <memory>
#include <random>
#include "IGame.hpp"
#include "HighScore.hpp"

using namespace std::chrono_literals;

namespace Arcade {

class Menu : public IGame {

private:

    int m_score;
    unsigned int m_iterator;
    std::shared_ptr<Arcade::Tile> m_arcade{};
    std::shared_ptr<Arcade::Tile> m_poster{};
    std::shared_ptr<Arcade::Text> m_menu_title{};
    std::shared_ptr<Arcade::Text> m_menu_enter{};
    std::shared_ptr<Arcade::Text> m_menu_exit{};
    std::vector<std::shared_ptr<Arcade::Text>> m_text_list{};
    std::vector<std::string> m_GameLibs{};
    std::shared_ptr<Arcade::Sound> m_sou{};
    HighScore m_high_scores{};

    Input m_ev{Input::NIL};

    static constexpr std::chrono::nanoseconds m_timestep{33ms};
    std::chrono::time_point<std::chrono::high_resolution_clock> m_clock{};

public:

    Menu();
    ~Menu() override = default;

    int getScore() override {return m_score;};
    void reset() override;
    std::vector<std::shared_ptr<IObject>> loop(Arcade::Input ev) override;
};

extern "C" Menu *entry_point();

}