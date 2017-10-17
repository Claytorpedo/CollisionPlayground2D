#pragma once
#ifndef _SAT_H
#define _SAT_H

#include "../Units.h"

class Polygon;

// Separating Axis Theorem tests.
// For all tests, "touching" polygons are not considered intersecting: they must overlap.
namespace sat {
	// Test if two convex polygons overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Polygon& first, const Polygon& second);
	// Test if two convex polygons with given positions overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos);
	// Test if two convex polygons with given positions overlap each other.
	// Gives the normal and distance that make up the minimum translation vector of separation to move the first polygon out of the second polygon.
	// Returns true if they overlap.
	bool performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos,
	                units::Coordinate2D& out_norm, units::Coordinate& out_dist);
	// Find when a collision will occur for one moving and one stationary polygon, and the normal of their collision, if they collide.
	// Takes polygons, their positions, and the first polygon's movement vector.
	// out_norm  - The collision normal for the first polygon (reverse it for the second).
	// out_t     - When they will collide in [0,1]. Multiply by delta vectors to get truncated movement vectors.
	//             If the polygons are already colliding before any movement, gives negative distance that, when made positive,
	//             forms the minimum translation vector with out_norm for the first polygon: separationVec = out_norm * (-out_t)
	// Returns true if the objects overlap or will overlap.
	bool performHybridSAT(const Polygon& first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
		const Polygon& second, const units::Coordinate2D& secondPos,
		units::Coordinate2D& out_norm, units::Fraction& out_interval);
	// Find when a collision will occur for two moving objects and the normal of their collision, if they collide.
	// Takes polygons, their positions, and movement vectors.
	// out_norm  - The collision normal for the first polygon (reverse it for the second).
	// out_t     - When they will collide in [0,1]. Multiply by delta vectors to get truncated movement vectors.
	//             If the polygons are already colliding before any movement, gives negative distance that, when made positive,
	//             forms the minimum translation vector with out_norm for the first polygon: separationVec = out_norm * (-out_t)
	// Returns true if the objects overlap or will overlap.
	bool performHybridSAT(const Polygon& first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
	                const Polygon& second, const units::Coordinate2D& secondPos, const units::Coordinate2D& secondDelta,
	                units::Coordinate2D& out_norm, units::Fraction& out_interval);
}

#endif // _SAT_H