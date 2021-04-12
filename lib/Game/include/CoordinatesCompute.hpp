/*
** EPITECH PROJECT, 2021
** arcade
** File description:
** CoordinatesCompute.hpp.h
*/

#pragma once

#include <cstddef>
#include <utility>

namespace Arcade {

/**
 * Computes the index corresponding to the coordinates x and y in the map
 *
 * @param x The x coordinate
 * @param y The y coordinate
 * @param lineLen length of a line
 * @return The corresponding index
 */
extern std::size_t computeIndex(unsigned int x, unsigned int y, unsigned int lineLen);

/**
 * Computes the x and y coordinates corresponding to the index in the map
 *
 * @param index the index of a character
 * @param lineLen length of a line
 * @return The corresponding x and y coordinates in an std::pair with first being x and second being y
 */
extern std::pair<unsigned int, unsigned int> computeCoordinates(unsigned int index, unsigned int lineLen);

}