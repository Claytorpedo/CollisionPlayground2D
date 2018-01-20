#pragma once
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
	geom::Polygon poly(const geom::gFloat minRad, const geom::gFloat maxRad, const std::size_t minVerts, const std::size_t maxVerts);
	// Generate a 2D coordinate within a given region.
	geom::Coord2 coord2(const geom::Rect& region);
	// Generate a gFloat within a given range [min, max).
	geom::gFloat gFloat(const geom::gFloat min, const geom::gFloat max);
}

#endif // INCLUDE_GAME_GENERATOR_HPP
