#pragma once
#ifndef _SAT_H
#define _SAT_H

#include "../Units.h"

class Shape;
class Polygon;

// Separating Axis Theorem tests.
// For all tests, "touching" shapes are not considered intersecting: they must overlap.
namespace sat {
	// Test if two shapes overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Shape* const first, const Shape* const second);
	// Test if two shapes with given positions overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Shape* const first, const units::Coordinate2D& firstPos, const Shape* const second, const units::Coordinate2D& secondPos);
	// Test if two convex polygons with given positions overlap each other.
	// Gives the normal and distance that make up the minimum translation vector of separation to move the first polygon out of the second polygon.
	// Returns true if they overlap.
	bool performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos,
	                units::Coordinate2D& out_norm, units::Coordinate& out_dist);
	// Describes the type of collision.
	enum class HybridResult {
		NONE, // No collision.
		MTV,  // Current collision.
		SWEEP // Future collision.
	};
	// Find when a collision will occur for one moving and one stationary polygon, and the normal of their collision, if they collide.
	// Takes polygons, their positions, and the first polygon's movement vector.
	// out_norm  - The collision normal for the first polygon (reverse it for the second).
	// out_t     - For SWEEP results: a value in range [0,1], indicating when along the delta vector the collision occurs.
	//             For MTV results: The polygons are already colliding. Gives distance to travel along the norm to separate.
	// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
	HybridResult performHybridSAT(const Polygon& first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
		const Polygon& second, const units::Coordinate2D& secondPos, units::Coordinate2D& out_norm, units::Fraction& out_t);
	// Find when a collision will occur for two moving objects and the normal of their collision, if they collide.
	// Takes polygons, their positions, and movement vectors.
	// out_norm  - The collision normal for the first polygon (reverse it for the second).
	// out_t     - For SWEEP results: a value in range [0,1], indicating when along the delta vectors the collision occurs.
	//             For MTV results: The polygons are already colliding. Gives distance to travel along the norm to separate.
	// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
	HybridResult performHybridSAT(const Polygon& first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
		const Polygon& second, const units::Coordinate2D& secondPos, const units::Coordinate2D& secondDelta, units::Coordinate2D& out_norm, units::Fraction& out_t);
}

#endif // _SAT_H