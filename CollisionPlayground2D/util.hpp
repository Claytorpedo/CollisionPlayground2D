#pragma once
#ifndef INCLUDE_GAME_UTIL_HPP
#define INCLUDE_GAME_UTIL_HPP

#include <SDL.h>

#include "units.hpp"
#include "constants.hpp"
#include "Geometry2D/Geometry.hpp"

namespace game {
	namespace util {
		const geom::gFloat COORD_TO_TILE = static_cast<geom::gFloat>(static_cast<geom::gFloat>(1) / TILE_SIZE);

		inline Pixel coordToPixel(const geom::gFloat c) {
			return static_cast<Pixel>(c < 0.0 ? std::ceil(c - 0.5) : std::floor(c + 0.5));
		}
		inline geom::gFloat pixelToCoord(const Pixel p) {
			return static_cast<geom::gFloat>(p);
		}
		inline Tile coordToTile(const geom::gFloat c) {
			return static_cast<Tile>(c * COORD_TO_TILE);
		}
		inline geom::gFloat tileToCoord(const Tile t) {
			return static_cast<geom::gFloat>(t * TILE_SIZE);
		}
		inline Pixel tileToPixel(const Tile t) {
			return coordToPixel(tileToCoord(t));
		}
		// Rounds to nearest whole number.
		inline MS fpsToMillis(const FPS fps) {
			return fps == 0 ? 0 : static_cast<MS>(std::round(static_cast<double>(SECOND_MILLIS) / fps));
		}
		// Rounds to nearest whole number.
		inline FPS millisToFPS(const MS ms) {
			return ms == 0 ? 0 : static_cast<FPS>(std::round(static_cast<double>(SECOND_MILLIS) / ms));
		}
		inline SDL_Point coord2DToSDLPoint(const geom::Coord2 c) {
			SDL_Point p = { static_cast<int>(c.x), static_cast<int>(c.y) };
			return p;
		}
	}
}


#endif // INCLUDE_GAME_UTIL_HPP
