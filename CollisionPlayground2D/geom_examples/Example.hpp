#pragma once
#ifndef INCLUDE_GAME_EXAMPLE_HPP
#define INCLUDE_GAME_EXAMPLE_HPP

#include "../units.hpp"

namespace game {
	class Input;
	class Graphics;

	class Example {
	public:
		virtual ~Example() {}
		virtual const bool update(Input input, MS elapsedTime);
		virtual const bool draw(Graphics graphics);
	};
}

#endif // INCLUDE_GAME_EXAMPLE_HPP
