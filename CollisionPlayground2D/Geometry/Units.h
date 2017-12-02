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

	typedef Vector2D<Coordinate>    Coordinate2D;
	typedef Vector2D<Pixel>         Pixel2D;
	typedef Vector2D<Tile>          Tile2D;
	typedef Vector2D<Velocity>      Velocity2D;
	typedef Vector2D<Acceleration>  Acceleration2D;
}

#endif // _UNITS_H_