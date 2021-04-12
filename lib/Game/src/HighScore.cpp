/*
** EPITECH PROJECT, 2021
** file_cpp
** File description:
** Score.cpp.cc
*/

#include "HighScore.hpp"

std::vector<std::string> HighScore::string_to_vector(std::string str, char separator)
{
    std::vector<std::string> array;
    std::string temp;
    size_t len = str.size();

    for (size_t i = 0; i < len; i++) {
        if (str[i] == separator) {
            if (!temp.empty()) {
                array.push_back(temp);
                temp.clear();
            }
        }
        else
            temp.push_back(str[i]);
    }
    if (!temp.empty())
        array.push_back(temp);
    return array;
}

HighScore::HighScore(const std::string &file)
{
    this->load(file);
}

void HighScore::load(const std::string &file)
{
    m_content.clear();
    m_file = "./scoreboard/";
    m_file += file.substr(file.find("arcade"));
    m_file.pop_back();
    m_file.pop_back();
    m_file += "txt";
    std::ifstream myfile(m_file);
    std::string line;

    while (std::getline(myfile, line))
        m_content.push_back(line);

    std::sort(std::begin(m_content), std::end(m_content),
              [&] (const std::string& lhs, const std::string& rhs) {
                return std::stoi(string_to_vector(lhs, ' ')[0]) > std::stoi(string_to_vector(rhs, ' ')[0]);
              });
}

void HighScore::writeNewScore()
{
    std::fstream fs(m_file, std::ios::in | std::ios::out | std::fstream::trunc);

    if (m_content.size() > 10)
        for (size_t i = 0; i < (m_content.size() - 10); i += 1)
            m_content.pop_back();

    for (auto &elem : m_content)
        fs << elem << "\n";
}

void HighScore::dump()
{
    for (auto &elem : m_content)
        std::cout << elem << std::endl;
}

void HighScore::setNewScore(std::string score)
{
    m_content.push_back(score);
    std::sort(std::begin(m_content), std::end(m_content),
              [&] (const std::string& lhs, const std::string& rhs) {
                return std::stoi(string_to_vector(lhs, ' ')[0]) > std::stoi(string_to_vector(rhs, ' ')[0]);
              });
}
