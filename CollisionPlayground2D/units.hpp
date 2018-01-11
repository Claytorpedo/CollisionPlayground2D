#pragma once
#ifndef INCLUDE_GAME_UNITS_HPP
#define INCLUDE_GAME_UNITS_HPP

#include "Geometry2D/Geometry.hpp"

namespace game {
	typedef int          Pixel;
	typedef int          Tile;
	typedef unsigned int MS;
	typedef unsigned int FPS;
	typedef geom::gFloat Velocity;     // Coordinate / MS
	typedef geom::gFloat Acceleration; // Coordinate / MS / MS

	typedef geom::Vec2<Pixel>         Pixel2D;
	typedef geom::Vec2<Tile>          Tile2D;
	typedef geom::Vec2<Velocity>      Velocity2D;
	typedef geom::Vec2<Acceleration>  Acceleration2D;
}

#endif // INCLUDE_GAME_UNITS_HPP