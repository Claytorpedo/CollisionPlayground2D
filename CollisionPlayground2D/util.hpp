#ifndef INCLUDE_GAME_UTIL_HPP
#define INCLUDE_GAME_UTIL_HPP

#include <SDL.h>

#include "units.hpp"
#include "constants.hpp"
#include "Geometry2D/Geometry.hpp"

namespace game::util {
inline Pixel coordToPixel(const ctp::gFloat c) {
	return static_cast<Pixel>(c < 0.0 ? std::ceil(c - 0.5) : std::floor(c + 0.5));
}
inline ctp::gFloat pixelToCoord(const Pixel p) {
	return static_cast<ctp::gFloat>(p);
}
// Rounds to nearest whole number.
inline MS fpsToMillis(const FPS fps) {
	return fps == 0 ? 0 : static_cast<MS>(std::round(static_cast<double>(SECOND_MILLIS) / fps));
}
// Rounds to nearest whole number.
inline FPS millisToFPS(const MS ms) {
	return ms == 0 ? 0 : static_cast<FPS>(std::round(static_cast<double>(SECOND_MILLIS) / ms));
}
inline SDL_Point coord2DToSDLPoint(const ctp::Coord2 c) {
	SDL_Point p = {static_cast<int>(c.x), static_cast<int>(c.y)};
	return p;
}
}

#endif // INCLUDE_GAME_UTIL_HPP
