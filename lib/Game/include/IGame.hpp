#pragma once

#include <vector>
#include <memory>
#include <chrono>
#include "Input.hpp"
#include "Object.hpp"

namespace Arcade {

using clock = std::chrono::high_resolution_clock;

class IGame {

public:

    virtual ~IGame() = default;

    /**
     *  @param event Arcade::Input event received to be processed by the game
	 * 	@return a vector of Arcade::IObject to be draw into the current graphical library
	 */
    virtual std::vector<std::shared_ptr<Arcade::IObject>> loop(Arcade::Input event) = 0;
    /**
	 * 	@return the actual score of the current game library
	 */
    virtual int getScore() = 0;
    /**
     *  Reset the current game library
	 */
    virtual void reset() = 0;

};

}