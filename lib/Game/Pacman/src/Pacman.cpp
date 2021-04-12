#include <fstream>
#include <sstream>
#include <iostream>
#include "Pacman.hpp"
#include "CoordinatesCompute.hpp"
#include "Exception.hpp"

extern "C" Arcade::Pacman *Arcade::entry_point()
{
    return new Arcade::Pacman;
}

Arcade::Pacman::Pacman() : m_gen(m_rd())
{
    Arcade::setTileSize(30);
    std::ifstream stream("assets/Pacman/map.txt");
    std::ostringstream content;

    if (stream.fail())
        throw Arcade::MissingAsset("Missing map for Pacman");
    content << stream.rdbuf();
    m_map.assign(content.str());
    std::erase_if(m_map, [](char c) { return (c == 'Z'); });
    for (auto &c : m_map)
        if (c == '\n')
            m_nbLines += 1;
    while (m_map[m_lineLen] != '\n')
        m_lineLen += 1;

    int x = 0;
    int y = 0;
    for (auto &c : m_map) {
        if (c == '\n') {
            y += 1;
            x = 0;
            continue;
        } else if (c == m_pacman1) {
            m_pacman_buf = std::make_shared<Arcade::Tile>(m_pacman1Asset, m_pacman1, YELLOW, x, y);
            m_x = x;
            m_y = y;
        } else if (c == m_wall)
            m_wall_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/wall.png", m_wall, BLUE, x, y));
        else if (c == m_phantom) {
            m_phantoms_buf.push_back(std::make_shared<Arcade::Tile>(m_phantomsAssets[m_phantoms_buf.size()],
                                                                    m_phantom, m_phantomsColors[m_phantoms_buf.size()],
                                                                    x, y));
            m_phantomInitCoords.emplace_back(std::make_pair<unsigned int, unsigned int>(x, y));
        } else if (c == m_dot)
            m_dots_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/dot.png", m_dot, WHITE, x, y));
        else if (c == m_pacGum)
            m_pacGum_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/pac_gum.png", m_pacGum, WHITE, x, y));
        else if (c == m_door)
            m_doors_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/door.png", m_door, YELLOW, x, y));
        x += 1;
    }
    m_score_MAIN = std::make_shared<Text>("SCORE", WHITE, 30, 14);
    m_score_DATA = std::make_shared<Text>(std::to_string(m_score), BLUE, 30, 15);
    m_move_sound = std::make_shared<Arcade::Sound>("assets/Pacman/chomp.wav");
}

void Arcade::Pacman::reset()
{
    m_death_sound_pop = false;
    m_first_loop = true;
    m_score = 0;
    m_dirX = 0;
    m_dirY = 0;
    m_vulnerableTicks = 0;
    m_ev = Arcade::Input::NIL;
    m_clock = clock::now();
    m_ticks = 0;
    m_pacmanMoved = false;
    m_phantomTicks = 0;
    m_freePhantoms = false;
    m_readyPhantoms = false;
    m_killed.fill(false);
    m_move.fill(false);
    m_wall_buf.clear();
    m_phantoms_buf.clear();
    m_dots_buf.clear();
    m_pacGum_buf.clear();
    m_doors_buf.clear();
    m_vulnerable.fill(false);
    m_phantomInitCoords.clear();
    m_gameOver = false;
    m_phantomMovements.fill(std::make_pair(0., 0.));
    m_wayOutIdx = 0;
    m_gameOverTicks = 0;

    std::ifstream stream("assets/Pacman/map.txt");
    std::ostringstream content;

    if (stream.fail())
        throw Arcade::MissingAsset("Missing map for Pacman");
    content << stream.rdbuf();
    m_map.assign(content.str());
    std::erase_if(m_map, [](char c) { return (c == 'Z'); });

    int x = 0;
    int y = 0;
    for (auto &c : m_map) {
        if (c == '\n') {
            y += 1;
            x = 0;
            continue;
        } else if (c == m_pacman1) {
            m_pacman_buf = std::make_shared<Arcade::Tile>(m_pacman1Asset, m_pacman1, YELLOW, x, y);
            m_x = x;
            m_y = y;
        } else if (c == m_wall)
            m_wall_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/wall.png", m_wall, BLUE, x, y));
        else if (c == m_phantom) {
            m_phantoms_buf.push_back(std::make_shared<Arcade::Tile>(m_phantomsAssets[m_phantoms_buf.size()],
                                                                    m_phantom, m_phantomsColors[m_phantoms_buf.size()],
                                                                    x, y));
            m_phantomInitCoords.emplace_back(std::make_pair<unsigned int, unsigned int>(x, y));
        } else if (c == m_dot)
            m_dots_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/dot.png", m_dot, WHITE, x, y));
        else if (c == m_pacGum)
            m_pacGum_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/pac_gum.png", m_pacGum, WHITE, x, y));
        else if (c == m_door)
            m_doors_buf.push_back(std::make_shared<Arcade::Tile>("assets/Pacman/door.png", m_door, YELLOW, x, y));
        x += 1;
    }
}

int Arcade::Pacman::movements(int dirX, int dirY, int rotation)
{
    float x = m_x + dirX;
    float y = m_y + dirY;

    m_dirX = dirX;
    m_dirY = dirY;
    if (x == -1) {
        x = m_lineLen - 1;
        m_pacman_buf->setPosition(x, y);
        m_dirX = 0;
        m_dirY = 0;
    } else if (x == m_lineLen) {
        x = 0;
        m_pacman_buf->setPosition(x, y);
        m_dirX = 0;
        m_dirY = 0;
    }
    if ((dirX != 0 && dirY != 0) || y > m_nbLines) {
        m_dirX = 0;
        m_dirY = 0;
        return (-1);
    }
    m_pacman_buf->setRotation(rotation);
    std::size_t pos = computeIndex(x, y, m_lineLen);
    if (m_map[pos] == m_dot) {
        m_score += 1;
        int i = 0;
        for (auto &dot : m_dots_buf) {
            auto coords = dot->getPosition();
            if (coords.first == x && coords.second == y) {
                m_dots_buf.erase(m_dots_buf.cbegin() + i);
                break;
            }
            i += 1;
        }
    } else if (m_map[pos] == m_pacGum) {
        int i = 0;
        for (auto &pacGum : m_pacGum_buf) {
            auto pacGumPos = pacGum->getPosition();
            if (pacGumPos.first == x && pacGumPos.second == y) {
                m_pacGum_buf.erase(m_pacGum_buf.cbegin() + i);
                break;
            }
            i += 1;
        }
        i = 0;
        for (auto &&elem : m_vulnerable) {
            elem = true;
            m_phantoms_buf[i]->setSymbol(m_vulnerablePhantom);
            m_phantoms_buf[i]->setPath("assets/Pacman/kill_enemy.png");
            i += 1;
        }
        m_vulnerableTicks = 600;
    } else if (m_map[pos] == m_phantom)
        return (1);
    else if (m_map[pos] == m_vulnerablePhantom) {
        int nb = 0;
        for (auto &&elem : m_vulnerable)
            if (!elem)
                nb += 1;
        m_score += 200 + (200 * nb);
        for (std::size_t i = 0; i < m_vulnerable.size(); i++) {
            auto phantomPos = m_phantoms_buf[i]->getPosition();
            if (phantomPos.first == x && phantomPos.second == y) {
                auto oldPos = m_phantoms_buf[i]->getPosition();
                m_vulnerable[i] = false;
                m_phantoms_buf[i]->setPath(m_phantomsAssets[i]);
                m_phantoms_buf[i]->setSymbol(m_phantom);
                m_phantoms_buf[i]->setPosition(m_phantomInitCoords[i].first, m_phantomInitCoords[i].second);
                m_map[computeIndex(oldPos.first, oldPos.second, m_lineLen)] = m_oldChar[i];
                m_map[computeIndex(m_phantomInitCoords[i].first, m_phantomInitCoords[i].second, m_lineLen)] = m_phantom;
                m_oldChar[i] = ' ';
                m_killed[i] = true;
            }
        }
    } else if (m_map[pos] == m_wall || m_map[pos] == '_') {
        m_dirX = 0;
        m_dirY = 0;
        return (0);
    }
    m_map[computeIndex(m_x, m_y, m_lineLen)] = ' ';
    m_map[pos] = 'C';
    m_pacman_buf->setSymbol(m_pacman_buf->getSymbol() == m_pacman1 ? m_pacman2 : m_pacman1);
    m_pacman_buf->setPath(m_pacman_buf->getPath() == "assets/Pacman/pacman1.png" ? m_pacman2Asset :
    m_pacman1Asset);
    m_x = x;
    m_y = y;
    m_pacmanMoved = true;
    return (0);
}

void Arcade::Pacman::getPhantomOut(int idx)
{
    auto pos = m_phantoms_buf[idx]->getPosition();
    auto nextIndex = computeIndex(pos.first + m_outPos[idx][0], pos.second + m_outPos[idx][1], m_lineLen);
    m_map[computeIndex(pos.first, pos.second, m_lineLen)] = m_oldChar[idx];
    m_oldChar[idx] = m_map[nextIndex];
    m_map[nextIndex] = m_phantoms_buf[idx]->getSymbol();
    m_phantoms_buf[idx]->setPosition(pos.first + m_outPos[idx][0], pos.second + m_outPos[idx][1]);
}

std::vector<std::shared_ptr<Arcade::IObject>> Arcade::Pacman::loop(Arcade::Input ev)
{
    int ret = 0;

    if (ev != Input::NIL)
        m_ev = ev;
    if (clock::now() - m_clock < m_timestep)
        return (std::vector<std::shared_ptr<Arcade::IObject>>{});
    if (m_gameOver && m_gameOverTicks == 6) {
        m_gameOverTicks = 0;
        return (gameOver());
    } else if (m_gameOver) {
        m_gameOverTicks += 1;
        return (std::vector<std::shared_ptr<Arcade::IObject>>{});
    }
    m_ticks += 1;
    m_phantomTicks += 1;

    if (m_phantomTicks == 300) {
        m_freePhantoms = true;
        m_phantomTicks = 0;
    } else if (m_freePhantoms && m_phantomTicks == 10 && !m_readyPhantoms) {
        for (int i = 0; i < 4; i++) {
            getPhantomOut(i);
        }
        m_readyPhantoms = true;
        m_phantomTicks = 0;
    } else if (m_readyPhantoms && m_ticks == 10)
        if (phantomMovementsRandom() == 1) {
            m_gameOver = true;
            return (gameOver());
        }

    if (m_vulnerableTicks != 0) {
        m_vulnerableTicks -= 1;
        if (m_vulnerableTicks == 0) {
            int i = 0;
            for (auto &&elem : m_vulnerable) {
                elem = false;
                m_phantoms_buf[i]->setSymbol(m_phantom);
                m_phantoms_buf[i]->setPath(m_phantomsAssets[i]);
                i += 1;
                if (m_killed[i]) {
                    auto pos = m_phantoms_buf[i]->getPosition();
                    m_phantoms_buf[i]->setPosition(m_phantomInitCoords[i].first + m_outPos[i][0],
                                                   m_phantomInitCoords[i].second + m_outPos[i][1]);
                    m_map[computeIndex(pos.first, pos.second, m_lineLen)] = ' ';
                    m_map[computeIndex(m_phantomInitCoords[i].first + m_outPos[i][0],
                                       m_phantomInitCoords[i].second + m_outPos[i][1], m_lineLen)] = m_phantom;
                }
            }
        }
    }
    if (m_ticks == 10) {
        m_dirX = 0;
        m_dirY = 0;
        switch (m_ev) {
            case Input::UP:
                ret = movements(0, -1, 270);
                break;
            case Input::DOWN:
                ret = movements(0, 1, 90);
                break;
            case Input::LEFT:
                ret = movements(-1, 0, 180);
                break;
            case Input::RIGHT:
                ret = movements(1, 0, 0);
                break;
            default:
                break;
        }
        m_ev = Input::NIL;
        m_ticks = 0;
    }
    auto pos = m_pacman_buf->getPosition();
    m_pacman_buf->setPosition(pos.first + static_cast<float>(m_dirX) / 10.,
                              pos.second + static_cast<float>(m_dirY) / 10.);
    /*int i = 0;
    for (auto &phantom : m_phantoms_buf) {
        std::cout << m_phantomMovements[i].first << '\n' << m_phantomMovements[i].second << std::endl;
        auto oldPos = phantom->getPosition();
        std::cout << oldPos.first << '\n' << oldPos.second << std::endl;
        phantom->setPosition(oldPos.first + m_phantomMovements[i].first, oldPos.second + m_phantomMovements[i].second);
        i += 1;
    }*/
    if (ret == -1)
        std::cout << "An error occurred" << std::endl;
    else if (ret == 1) {
        m_gameOver = true;
        return (gameOver());
    }
    m_clock = clock::now();
    return (generateBuffer());
}

int Arcade::Pacman::phantomMovementsRandom()
{
    int i = 0;
    int ret = 0;

    for (auto &phantom : m_phantoms_buf) {
        if (m_vulnerable[i] && !m_move[i]) {
            m_move[i] = true;
            continue;
        } else if (m_vulnerable[i] && m_move[i])
            m_move[i] = false;
        auto pos = phantom->getPosition();
        int index;
        float x = 0;
        float y = 0;

        int nb_loop = 0;
        do {
            if (nb_loop == 50)
                break;
            int rd_nb = m_distrib(m_gen);
            x = pos.first;
            y = pos.second;
            switch (rd_nb) {
                case 0:
                    x += 1;
                    break;
                case 1:
                    y -= 1;
                    break;
                case 2:
                    x -= 1;
                    break;
                case 3:
                    y += 1;
                    break;
                default:
                    break;
            }
            index = computeIndex(x, y, m_lineLen);
            if (m_map[index] == m_pacman1 && !m_vulnerable[i])
                ret = 1;
            else if (m_map[index] == m_pacman1 && m_vulnerable[i]) {
                auto pos = m_phantoms_buf[i]->getPosition();
                m_vulnerable[i] = false;
                m_phantoms_buf[i]->setPath(m_phantomsAssets[i]);
                m_phantoms_buf[i]->setSymbol(m_phantom);
                m_phantoms_buf[i]->setPosition(m_phantomInitCoords[i].first, m_phantomInitCoords[i].second);
                m_map[computeIndex(pos.first, pos.second, m_lineLen)] = m_oldChar[i];
                m_map[computeIndex(m_phantomInitCoords[i].first, m_phantomInitCoords[i].second, m_lineLen)] = m_phantom;
                m_oldChar[i] = ' ';
                m_killed[i] = true;
            }
            nb_loop += 1;
        } while (m_map[index] != ' ' && m_map[index] != m_dot && m_map[index] != m_pacGum && m_map[index] != m_pacman1);

        m_map[computeIndex(pos.first, pos.second, m_lineLen)] = m_oldChar[i];
        m_oldChar[i] = m_map[index];
        m_map[index] = phantom->getSymbol();
        phantom->setPosition(x, y);
        /*if (m_vulnerable[i])
            m_phantomMovements[i] = std::make_pair((x - pos.first) / 20., (y - pos.second) / 20.);
        else
            m_phantomMovements[i] = std::make_pair((x - pos.first) / 10., (y - pos.second) / 10.);*/
        i += 1;
    }
    return (ret);
}

std::vector<std::shared_ptr<Arcade::IObject>> Arcade::Pacman::gameOver()
{
    auto buf = generateBuffer();
    buf.push_back(std::make_shared<Arcade::Text>("Game Over!", Arcade::Color::RED, 30, 17));
    buf.push_back(std::make_shared<Arcade::Text>("Press R to restart the game", Arcade::Color::YELLOW, 30, 19));
    buf.push_back(std::make_shared<Arcade::Text>("Press M to go back to the menu", Arcade::Color::CYAN, 30, 21));
    if (!m_death_sound_pop) {
        buf.push_back(std::make_shared<Arcade::Sound>("assets/Pacman/death.wav"));
        m_death_sound_pop = true;
    }
    return (buf);
}

std::vector<std::shared_ptr<Arcade::IObject>> Arcade::Pacman::generateBuffer()
{
    std::vector<std::shared_ptr<Arcade::IObject>> buf{};
    if (m_first_loop) {
        buf.push_back(std::make_shared<Arcade::Sound>("assets/Pacman/beginning.wav"));
        m_first_loop = false;
    }

    if (m_pacmanMoved) {
        buf.push_back(m_move_sound);
        m_pacmanMoved = false;
    }
    for (auto &elem : m_wall_buf)
        buf.push_back(elem);
    for (auto &elem : m_dots_buf)
        buf.push_back(elem);
    for (auto &elem : m_pacGum_buf)
        buf.push_back(elem);
    for (auto &elem : m_doors_buf)
        buf.push_back(elem);
    for (auto &elem : m_phantoms_buf)
        buf.push_back(elem);
    buf.push_back(m_pacman_buf);
    buf.push_back(m_score_MAIN);
    m_score_DATA->setText(std::to_string(m_score));
    buf.push_back(m_score_DATA);
    return (buf);
}
