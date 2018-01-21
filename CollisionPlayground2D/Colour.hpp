#pragma once
#ifndef INCLUDE_COLOURS_HPP
#define INCLUDE_COLOURS_HPP

#include <SDL.h>

class Colour {
public:
	static const Colour DARK_GREY;
	static const Colour RED;
	static const Colour YELLOW;
	static const Colour ORANGE;
	static const Colour FUCHSIA;
	static const Colour LIGHT_BLUE;
	static const Colour CYAN;
	static const Colour LIGHT_GREEN;

	Uint8 r, g, b, a;
	Colour() : r(0), g(0), b(0), a(0) {}
	Colour(Uint8 r, Uint8 g, Uint8 b) : r(r), g(g), b(b), a(255) {}
	Colour(Uint8 r, Uint8 g, Uint8 b, Uint8 a) : r(r), g(g), b(b), a(a) {}
};

#endif // INCLUDE_COLOURS_HPP
