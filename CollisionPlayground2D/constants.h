#pragma once
#ifndef INCLUDE_CONSTANTS_HPP
#define INCLUDE_CONSTANTS_HPP

#include "units.h"

namespace game {
	const Pixel SCREEN_WIDTH = 1280;
	const Pixel SCREEN_HEIGHT = 720;

	const MS    SECOND_MILLIS = 1000; // Milliseconds per second.
	const MS    MAX_FRAME_TIME = 50;  // The maximum lengh one frame of gameplay can last in MS.
	const Pixel TILE_SIZE = 16;
}

#endif // INCLUDE_CONSTANTS_HPP
