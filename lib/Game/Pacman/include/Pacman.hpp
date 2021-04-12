#pragma once

#include <memory>
#include <random>
#include "IGame.hpp"

using namespace std::chrono_literals;

namespace Arcade {

class Pacman : public IGame {

private:

    int m_score{};
    float m_x{};
    float m_y{};
    int m_dirX{};
    int m_dirY{};

    unsigned int m_vulnerableTicks{};

    Input m_ev{Input::NIL};

    std::random_device m_rd{};
    std::mt19937 m_gen;
    std::uniform_int_distribution<> m_distrib{0, 3};

    static constexpr std::chrono::nanoseconds m_timestep{33ms};
    std::chrono::time_point<std::chrono::high_resolution_clock> m_clock{};
    int m_ticks{};

    bool m_pacmanMoved{};

    int m_phantomTicks{};
    bool m_freePhantoms{};
    bool m_readyPhantoms{};
    std::array<bool, 4> m_killed{};
    std::array<bool, 4> m_move{};

    std::shared_ptr<Arcade::Tile> m_pacman_buf{};
    std::vector<std::shared_ptr<Arcade::Tile>> m_wall_buf{};
    std::vector<std::shared_ptr<Arcade::Tile>> m_phantoms_buf{};
    std::vector<std::shared_ptr<Arcade::Tile>> m_dots_buf{};
    std::vector<std::shared_ptr<Arcade::Tile>> m_pacGum_buf{};
    std::vector<std::shared_ptr<Arcade::Tile>> m_doors_buf{};

    std::shared_ptr<Arcade::Text> m_score_MAIN{};
    std::shared_ptr<Arcade::Text> m_score_DATA{};

    std::shared_ptr<Arcade::Sound> m_move_sound{};
    bool m_death_sound_pop = false;
    bool m_first_loop = true;

    std::array<bool, 4> m_vulnerable{};
    std::vector<std::pair<float, float>> m_phantomInitCoords{};

    std::string m_map{};
    unsigned int m_nbLines{};
    unsigned int m_lineLen{};

    bool m_gameOver{};
    int m_gameOverTicks{};

    std::array<std::pair<float, float>, 4> m_phantomMovements{};

    const int m_outPos[4][2] = {{1, -2}, {0, -3}, {-1, -3}, {0, -4}};
    char m_oldChar[4] = {' ', ' ', ' ', ' '};
    int m_wayOutIdx{};

    const char m_phantomsAssets[4][31] = {"assets/Pacman/blue_enemy.png",
                                          "assets/Pacman/orange_enemy.png",
                                          "assets/Pacman/pink_enemy.png",
                                          "assets/Pacman/red_enemy.png"};
    static constexpr Color m_phantomsColors[] = {CYAN, YELLOW, GREEN, RED};

    static constexpr auto m_pacman1Asset = "assets/Pacman/pacman1.png";
    static constexpr auto m_pacman2Asset = "assets/Pacman/pacman2.png";

    static constexpr char m_pacman1 = 'C';
    static constexpr char m_pacman2 = 'c';
    static constexpr char m_wall = 'X';
    static constexpr char m_phantom = 'A';
    static constexpr char m_vulnerablePhantom = 'V';
    static constexpr char m_dot = '.';
    static constexpr char m_pacGum = 'o';
    static constexpr char m_door = '_';

    /**
     * Checks for collisions with a wall or an enemy
     *
     * @param dirX The direction on the X axis
     * @param dirY The direction on the Y axis
     * @param rotation The angle to rotate the Pacman Sprite
     * @return -1 on error,
     *          0 if no collision will happen,
     *          1 if there is an enemy
     */
    int movements(int dirX, int dirY, int rotation);

    /**
     * Movements for orange Phantom (Clyde)
     * His movements are random
     */
    int phantomMovementsRandom();

    /**
     * Generates a buffer to return to the core that is going to be passed to the graphical library in order for it
     * to load the textures
     *
     * @return A buffer containing all the assets to load
     */
    std::vector<std::shared_ptr<Arcade::IObject>> generateBuffer();

    /**
     * Teleports the phantom to its position outside of the box
     *
     * @param idx The index of the phantom to teleport
     */
    void getPhantomOut(int idx);

    /**
     * Function executed to display a game over screen
     *
     * @return A buffer containing the assets to display
     */
    std::vector<std::shared_ptr<Arcade::IObject>> gameOver();

public:

    Pacman();
    ~Pacman() override = default;

    int getScore() override {return m_score;};
    void reset() override;
    std::vector<std::shared_ptr<IObject>> loop(Arcade::Input ev) override;

};

extern "C" Pacman *entry_point();

}