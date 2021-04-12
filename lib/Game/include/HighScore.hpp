/*
** EPITECH PROJECT, 2021
** file_cpp
** File description:
** HighScore.hpp.h
*/

#pragma once

#include <iostream>
#include <vector>
#include <fstream>
#include <algorithm>

class HighScore {
private:
    std::string m_file {};
    std::vector<std::string> m_content {};
    std::vector<std::string> string_to_vector(std::string str, char separator);

public:
    HighScore(const std::string &file);
    HighScore() = default;
    ~HighScore() = default;

    void load(const std::string &file);
    void writeNewScore();
    void setNewScore(std::string score);
    void dump();
    std::vector<std::string> getContent() { return m_content; }
};