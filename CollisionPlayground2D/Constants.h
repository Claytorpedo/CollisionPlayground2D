#ifndef _CONSTANTS_H
#define _CONSTANTS_H

#include <string>
#include "Units.h"

namespace constants {

	const units::Coordinate SCREEN_WIDTH = 1280; // Screen probably shouldn't be constant, but fine for now.
	const units::Coordinate SCREEN_HEIGHT = 720;

	const units::MS         SECOND_MILLIS = 1000;
	const units::MS         MAX_FRAME_TIME = 50; // The maximum lengh one frame of gameplay can last in MS.
	const units::Coordinate EPSILON = 0.00001f;  // Float equality tolerance.
	const units::Pixel      TILE_SIZE = 16;

	const units::Fraction   PI  = 3.141592653589793238f;
	const units::Fraction   TAU = 6.283185307179586477f; // 2PI

	// For the loading textures to work, the order of this enum and the following string paths must be the same.
	enum ObjectTextures {
		OBJECT_TEXTURE_BACKGROUND,
		OBJECT_TEXTURE_BUG,
		OBJECT_TEXTURE_DIAMOND,
		OBJECT_TEXTURE_GHOST,
		OBJECT_TEXTURE_KEY,
		OBJECT_TEXTURE_LOCK,
		OBJECT_TEXTURE_TOTAL
	};

	const std::string MAPS_PATH = "maps/";
	const std::string MAPS_EXT = ".map";

	// Paths for the above object Textures. Must be in the same order as ObjectTextures.
	const std::string SPRITE_PATH = "sprites/";
	const std::string filePaths[] = {
		SPRITE_PATH + "blockTilesThing.png", 
		SPRITE_PATH + "bug.png", 
		SPRITE_PATH + "diamond.png", 
		SPRITE_PATH + "ghost.png", 
		SPRITE_PATH + "key.png",
		SPRITE_PATH + "lock.png", 
	};

	enum DirectionType {
		DIRECTION_TYPE_NONE,
		DIRECTION_TYPE_UP,
		DIRECTION_TYPE_UP_RIGHT,
		DIRECTION_TYPE_RIGHT,
		DIRECTION_TYPE_DOWN_RIGHT,
		DIRECTION_TYPE_DOWN,
		DIRECTION_TYPE_DOWN_LEFT,
		DIRECTION_TYPE_LEFT,
		DIRECTION_TYPE_UP_LEFT,
		DIRECTION_TYPE_TOTAL
	};
}

#endif // _CONSTANTS_H