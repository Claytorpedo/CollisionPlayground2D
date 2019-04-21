#pragma once
#ifndef INCLUDE_GAME_UNITS_HPP
#define INCLUDE_GAME_UNITS_HPP

#include <cstdint>

#include <Geometry2D/Geometry.hpp>

namespace game {
	using Pixel = int32_t;
	using MS = uint32_t;
	using FPS = uint32_t;
	using Velocity = geom::gFloat;     // Coordinate / MS
	using Acceleration = geom::gFloat; // Coordinate / MS / MS

	using Pixel2D = geom::Vec2<Pixel>;
	using Velocity2D = geom::Vec2<Velocity>;
	using Acceleration2D = geom::Vec2<Acceleration>;
}

#endif // INCLUDE_GAME_UNITS_HPP
