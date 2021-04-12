#include "Core.hpp"
#include "Exception.hpp"
#include <iostream>

int main(int ac, char **av)
{
    try {
        Arcade::Core app(ac, av);
    } catch(const Arcade::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    } catch(const std::exception &e) {
        std::cerr << e.what() << std::endl;
        return 84;
    }
    return EXIT_SUCCESS;
}