#include <fstream>
#include <iostream>
#include <sstream>
#include "Snake.hpp"
#include "CoordinatesCompute.hpp"
#include "Exception.hpp"

extern "C" Arcade::Snake *Arcade::entry_point()
{
    return new Arcade::Snake;
}

Arcade::Snake::Snake() : m_dirX(-1), m_dirY(0), m_rotation(90), m_gen(m_rd())
{
    Arcade::setTileSize(50);
    m_eat_sound = std::make_shared<Arcade::Sound>("assets/Snake/eat.wav");
    m_death_sound = std::make_shared<Arcade::Sound>("assets/Snake/death.wav");
    std::ifstream stream("assets/Snake/map.txt");
    std::ostringstream content;

    if (stream.fail())
        throw Arcade::MissingAsset("Missing map for Nibbler");
    content << stream.rdbuf();
    m_map.assign(content.str());
    for (auto &c : m_map)
        if (c == '\n')
            m_nbLines += 1;
    while (m_map[m_lineLen] != '\n')
        m_lineLen += 1;

    m_distrib.param(std::uniform_int_distribution<>::param_type(0, m_map.size()));

    int x = 0;
    int y = 0;
    for (auto &c : m_map) {
        if (c == '\n') {
            y += 1;
            x = 0;
            continue;
        } else if (c == m_snake) {
            m_buf_snake.push_back(std::make_shared<Tile>("assets/Snake/snake.png", m_snake, BLUE, x, y));
        } else if (c == m_wall) {
            m_buf_wall.push_back(std::make_shared<Tile>("assets/Snake/wall.png", m_wall, GREEN, x, y));
        } else if (c == m_snakeHead) {
            m_buf_snake.push_back(std::make_shared<Tile>("assets/Snake/snake_head.png", m_snakeHead, MAGENTA, x, y));
            m_x = x;
            m_y = y;
        }
        x += 1;
    }
    m_score_MAIN = std::make_shared<Text>("SCORE", WHITE, 23, 8);
    m_score_DATA = std::make_shared<Text>(std::to_string(m_score), BLUE, 23, 9);
    generateNewApple();
}

void Arcade::Snake::reset()
{
    m_buf_snake.clear();
    m_buf_apple.clear();
    m_buf_wall.clear();

    m_first_death_loop = true;
    m_gameOverTicks = 0;

    std::ifstream stream("assets/Snake/map.txt");
    std::ostringstream content;

    if (stream.fail())
        throw Arcade::MissingAsset("Missing map for Nibbler");
    content << stream.rdbuf();
    m_map.assign(content.str());

    int x = 0;
    int y = 0;
    for (auto &c : m_map) {
        if (c == '\n') {
            y += 1;
            x = 0;
            continue;
        } else if (c == m_snake) {
            m_buf_snake.push_back(std::make_shared<Tile>("assets/Snake/snake.png", m_snake, BLUE, x, y));
        } else if (c == m_wall) {
            m_buf_wall.push_back(std::make_shared<Tile>("assets/Snake/wall.png", m_wall, GREEN, x, y));
        } else if (c == m_snakeHead) {
            m_buf_snake.push_back(std::make_shared<Tile>("assets/Snake/snake_head.png", m_snakeHead, MAGENTA, x, y));
            m_x = x;
            m_y = y;
        }
        x += 1;
    }
    generateNewApple();
    m_clock = clock::now();
    m_ev = Arcade::Input::NIL;
    m_score = 0;
    m_dirX = -1;
    m_dirY = 0;
    m_rotation = 90;
    m_gameOver = false;
}

int Arcade::Snake::movements()
{
    int ret = 0;
    unsigned int x = m_x + m_dirX;
    unsigned int y = m_y + m_dirY;

    if ((m_dirX != 0 && m_dirY != 0) || x >= m_lineLen || y >= m_nbLines)
        return (-1);
    std::size_t pos = computeIndex(x, y, m_lineLen);
    m_buf_snake[0]->setRotation(m_rotation);
    if (m_map[pos] == m_wall || m_map[pos] == m_snake)
        return (1);
    else if (m_map[pos] == m_apple) {
        ret = 2;
        m_score += 1;
        int i = 0;
        for (auto &apple : m_buf_apple) {
            auto applePos = apple->getPosition();
            if (applePos.first == x && applePos.second == y) {
                m_buf_apple.erase(m_buf_apple.cbegin() + i);
                break;
            }
            i += 1;
        }
        m_buf_snake.push_back(std::make_shared<Arcade::Tile>("assets/Snake/snake.png", m_snake, BLUE, 0, 0));
        generateNewApple();
    } else {
        auto tailPos = m_buf_snake[m_buf_snake.size() - 1]->getPosition();
        m_map[computeIndex(tailPos.first, tailPos.second, m_lineLen)] = ' ';
    }
    for (std::size_t i = m_buf_snake.size() - 1; i > 0; i--) {
        auto prevPos = m_buf_snake[i - 1]->getPosition();
        m_buf_snake[i]->setPosition(prevPos.first, prevPos.second);
        m_map[computeIndex(prevPos.first, prevPos.second, m_lineLen)] = 'S';
    }
    m_buf_snake[0]->setPosition(x, y);
    m_map[computeIndex(x, y, m_lineLen)] = 'N';
    m_x = x;
    m_y = y;
    return ret;
}

void Arcade::Snake::generateNewApple()
{
    int index;

    do {
        index = m_distrib(m_gen);
    } while (m_map[index] != ' ');
    m_map[index] = m_apple;

    auto pos = computeCoordinates(index, m_lineLen);
    m_buf_apple.push_back(std::make_shared<Arcade::Tile>("assets/Snake/apple.png", m_apple, RED, pos.first, pos
    .second));
}

std::vector<std::shared_ptr<Arcade::IObject>> Arcade::Snake::loop(Arcade::Input ev)
{
    if (ev != Arcade::Input::NIL)
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

    if (m_ticks == 10) {
        switch (m_ev) {
            case Input::UP:
                if (m_dirY == 1)
                    break;
                m_dirX = 0;
                m_dirY = -1;
                m_rotation = 180;
                break;
            case Input::DOWN:
                if (m_dirY == -1)
                    break;
                m_dirX = 0;
                m_dirY = 1;
                m_rotation = 0;
                break;
            case Input::LEFT:
                if (m_dirX == 1)
                    break;
                m_dirX = -1;
                m_dirY = 0;
                m_rotation = 90;
                break;
            case Input::RIGHT:
                if (m_dirX == -1)
                    break;
                m_dirX = 1;
                m_dirY = 0;
                m_rotation = 270;
                break;
            default:
                break;
        }
        m_ticks = 0;
        int ret = movements();
        if (ret == -1)
            std::cout << "An error occurred" << std::endl;
        else if (ret == 1) {
            m_gameOver = true;
            return (gameOver());
        }
        return (generateBuffer(ret));
    }
    m_clock = clock::now();
    return (generateBuffer());
}

std::vector<std::shared_ptr<Arcade::IObject>> Arcade::Snake::gameOver()
{
    auto buf = generateBuffer();
    if (m_first_death_loop) {
        buf.push_back(m_death_sound);
        m_first_death_loop = false;
    }
    buf.push_back(std::make_shared<Arcade::Text>("Game Over!", Arcade::Color::RED, 23, 11));
    buf.push_back(std::make_shared<Arcade::Text>("Press R to restart the game", Arcade::Color::YELLOW, 23, 13));
    buf.push_back(std::make_shared<Arcade::Text>("Press M to go back to the menu", Arcade::Color::CYAN, 23, 14));
    return (buf);
}

std::vector<std::shared_ptr<Arcade::IObject>> Arcade::Snake::generateBuffer(int ret)
{
    std::vector<std::shared_ptr<Arcade::IObject>> buf{};
    buf.reserve(m_buf_snake.size() + m_buf_apple.size() + m_buf_wall.size());
    if (ret == 2)
        buf.push_back(m_eat_sound);
    for (auto &elem : m_buf_wall)
        buf.push_back(elem);
    for (auto &elem : m_buf_snake)
        buf.push_back(elem);
    for (auto &elem : m_buf_apple)
        buf.push_back(elem);
    buf.push_back(m_score_MAIN);
    m_score_DATA->setText(std::to_string(m_score));
    buf.push_back(m_score_DATA);
    return (buf);
}
