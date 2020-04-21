#ifndef INCLUDE_GAME_GENERATOR_HPP
#define INCLUDE_GAME_GENERATOR_HPP

#include <random>

#include "Geometry2D/Geometry.hpp"

namespace gen {
static std::mt19937 rng;
void init();
// Generate a polygon.
// region is a bounding box defining the region to place the polygon's center in (part of the polygon can be outside this region).
// minRad and maxRad control how large the generated polygon will be.
// minVerts and maxVerts control how many vertices the generated polygon can have.
ctp::Polygon poly(const ctp::gFloat minRad, const ctp::gFloat maxRad, const std::size_t minVerts, const std::size_t maxVerts);
// Generate a 2D coordinate within a given region.
ctp::Coord2 coord2(const ctp::Rect& region);
// Generate a gFloat within a given range [min, max).
ctp::gFloat gFloat(const ctp::gFloat min, const ctp::gFloat max);
}

#endif // INCLUDE_GAME_GENERATOR_HPP
