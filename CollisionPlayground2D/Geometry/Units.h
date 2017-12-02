#ifndef _UNITS_H_
#define _UNITS_H_

#include "Vector2D.h"

namespace units {
	typedef float        Fraction;
	typedef float        Coordinate;
	typedef int          Pixel;
	typedef int          Tile;
	typedef unsigned int MS;
	typedef unsigned int FPS;
	typedef float        Velocity;     // Coordinate / MS
	typedef float        Acceleration; // Coordinate / MS / MS

	typedef geom::Vec2<Coordinate>    Coordinate2D;
	typedef geom::Vec2<Pixel>         Pixel2D;
	typedef geom::Vec2<Tile>          Tile2D;
	typedef geom::Vec2<Velocity>      Velocity2D;
	typedef geom::Vec2<Acceleration>  Acceleration2D;
}

#endif // _UNITS_H_