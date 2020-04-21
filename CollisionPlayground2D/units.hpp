#ifndef INCLUDE_GAME_UNITS_HPP
#define INCLUDE_GAME_UNITS_HPP

#include <cstdint>

#include <Geometry2D/Geometry.hpp>

namespace game {
using Pixel = std::int32_t;
using MS = std::uint32_t;
using FPS = std::uint32_t;
using Velocity = ctp::gFloat;     // Coordinate / MS
using Acceleration = ctp::gFloat; // Coordinate / MS / MS

using Velocity2D = ctp::Vec2<Velocity>;
using Acceleration2D = ctp::Vec2<Acceleration>;
}

#endif // INCLUDE_GAME_UNITS_HPP
