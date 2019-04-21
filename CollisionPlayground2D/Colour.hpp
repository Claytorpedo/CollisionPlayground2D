#pragma once
#ifndef INCLUDE_COLOURS_HPP
#define INCLUDE_COLOURS_HPP

#include <SDL.h>

struct Colour {
	static const Colour DARK_GREY;
	static const Colour RED;
	static const Colour YELLOW;
	static const Colour ORANGE;
	static const Colour FUCHSIA;
	static const Colour LIGHT_BLUE;
	static const Colour CYAN;
	static const Colour LIGHT_GREEN;

	Uint8 r{ 0 }, g{ 0 }, b{ 0 }, a{ 255 };
};

#endif // INCLUDE_COLOURS_HPP
