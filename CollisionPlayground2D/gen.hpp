#pragma once
#ifndef INCLUDE_GAME_GEN_HPP
#define INCLUDE_GAME_GEN_HPP

#include <random>

#include "Geometry2D/Geometry.hpp"

class Mover;
namespace game {
	// Generate a polygon.
	// region is a bounding box defining the region to place the polygon's center in (part of the polygon can be outside this region).
	// minRad and maxRad control how large the generated polygon will be.
	// minVerts and maxVerts control how many vertices the generated polygon can have.
	geom::Polygon genPoly(std::mt19937& rando, const geom::Rect& region,
		const geom::gFloat minRad, const geom::gFloat maxRad, const std::size_t minVerts, const std::size_t maxVerts);
}

#endif INCLUDE_GAME_GEN_HPP
