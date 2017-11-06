#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include "Units.h"

namespace constants {

	const units::Coordinate SCREEN_WIDTH = 1280;
	const units::Coordinate SCREEN_HEIGHT = 720;

	const units::MS         SECOND_MILLIS = 1000;
	const units::MS         MAX_FRAME_TIME = 50; // The maximum lengh one frame of gameplay can last in MS.
	const units::Coordinate EPSILON = 0.000001f; // Float equality tolerance.
	const units::Pixel      TILE_SIZE = 16;

	const units::Fraction   PI  = 3.141592653589793238f;
	const units::Fraction   TAU = 6.283185307179586477f; // 2PI
}

#endif // _CONSTANTS_H