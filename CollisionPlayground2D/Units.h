#pragma once
#ifndef _GAME_UNITS_H
#define _GAME_UNITS_H

#include "Geometry/Units.h"
#include "Geometry/Vector2D.h"

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

#endif // _GAME_UNITS_H