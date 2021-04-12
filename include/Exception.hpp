#pragma once

#include <exception>
#include <string>

namespace Arcade {

/**
 *  Exception class for Arcade
 */
class exception : public std::exception {

protected:

    const std::string m_error;

public:

    explicit exception(std::string error) : m_error(std::move(error)) {}
    [[nodiscard]] const char *what() const noexcept override {return m_error.c_str();}
};

/**
 *  Exception thrown when a missing asset is detected at runtime. 
 *  Inherits from Arcade::exception
 */
class MissingAsset : public exception {

public:

    explicit MissingAsset(std::string error) : exception(std::move(error)) {}
};

/**
 *  Exception thrown when an error occured when loading a dynamic library at runtime. 
 *  Inherits from Arcade::exception
 */
class DyLibException : public exception {

public:

    explicit DyLibException(std::string error) : exception(std::move(error)) {}
};

}