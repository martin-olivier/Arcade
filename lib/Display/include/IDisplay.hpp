#pragma once

#include <memory>
#include "Input.hpp"
#include "Object.hpp"

namespace Arcade
{
    class IDisplay
    {
	public:
        virtual ~IDisplay() = default;
		/**
		 * 	@return Arcade::Input that corresponds to the last event from the current graphical library
		 */
		virtual Arcade::Input event() = 0;
		/**
		 * 	clear the screen of the current graphical library
		 */
		virtual void clear() = 0;
		/**
		 * 	refresh the screen of the current graphical library
		 */
		virtual void refresh() = 0;
		/**
		 * 	Draw the Arcade::IObject into the current graphical library
		 * 	
		 * 	@param object to be draw into the current graphical library
		 */
        virtual void draw(std::shared_ptr<Arcade::IObject> object) = 0;
	};
}