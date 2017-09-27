#ifndef _UTIL_H_
#define _UTIL_H_

#include <cmath>
#include <SDL.h>

#include "Units.h"
#include "Constants.h"

namespace util {
	const units::Fraction COORD_TO_TILE = static_cast<units::Fraction>(static_cast<units::Fraction>(1) / constants::TILE_SIZE);

	inline units::Pixel coordToPixel( const units::Coordinate c ) { 
		return static_cast<units::Pixel>( c < 0.0 ? std::ceil(c - 0.5) : std::floor(c + 0.5)); 
	}
	inline units::Coordinate pixelToCoord( const units::Pixel p ) {
		return static_cast<units::Coordinate>(p);
	}
	inline units::Tile coordToTile( const units::Coordinate c ) {
		return static_cast<units::Tile>( c * COORD_TO_TILE ); 
	}
	inline units::Coordinate tileToCoord( const units::Tile t ) { 
		return static_cast<units::Coordinate>( t * constants::TILE_SIZE ); 
	}
	inline units::Pixel tileToPixel( const units::Tile t ) { 
		return coordToPixel(tileToCoord( t ) ); 
	}
	// Rounds to nearest whole number.
	inline units::MS fpsToMillis( const units::FPS fps ) {
		return fps == 0 ? 0 : static_cast<units::MS>( std::round(static_cast<double>(constants::SECOND_MILLIS) / fps ));
	}
	// Rounds to nearest whole number.
	inline units::FPS millisToFPS(const units::MS ms) {
		return ms == 0 ? 0 : static_cast<units::FPS>(std::round(static_cast<double>(constants::SECOND_MILLIS) / ms));
	}
	inline SDL_Point coord2DToSDLPoint( const units::Coordinate2D c ) {
		SDL_Point p = {static_cast<int>(c.x), static_cast<int>(c.y)};
		return p;
	}
	template<typename T>
	inline bool almostZero(const T t, const T tolerance=constants::EPSILON) { // Use for floating point numbers.
		return -tolerance < t && t < tolerance;
	}
	inline bool almostEquals(const units::Coordinate& a, const units::Coordinate& b, const units::Coordinate tolerance=constants::EPSILON) {
		return std::abs(a - b) < tolerance;
	}
	inline bool almostEquals(const units::Coordinate2D& a, const units::Coordinate2D& b, const units::Coordinate tolerance = constants::EPSILON) {
		return almostEquals(a.x, b.x, tolerance) && almostEquals(a.y, b.y, tolerance);
	}
	// Check if a value is between two bounds.
	template<typename T>
	inline T clamp(const T val, const T bound_one, const T bound_two) {
		T min, max;
		if (bound_one <= bound_two) {
			min = bound_one;
			max = bound_two;
		} else {
			max = bound_two;
			min = bound_one;
		}
		return val < min ? min : val > max ? max : val;
	}
}

#endif // _UTIL_H_