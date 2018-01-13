#pragma once
#ifndef INCLUDE_GAME_EXAMPLE_HPP
#define INCLUDE_GAME_EXAMPLE_HPP

#include "../units.hpp"

class Input;
class Graphics;
namespace game {
	class Example {
	public:
		virtual ~Example() {}
		virtual void update(Input& input, const MS elapsedTime) = 0;
		virtual void draw(Graphics& graphics) = 0;
		virtual void reset() = 0;
	};
}

#endif // INCLUDE_GAME_EXAMPLE_HPP
