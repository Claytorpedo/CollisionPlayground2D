#pragma once
#ifndef _SAT_H
#define _SAT_H

#include <vector>

#include "../Units.h"

class Shape;

// Separating Axis Theorem tests.
// For all tests, "touching" shapes are not considered intersecting: they must overlap.
namespace sat {
	// Test if two shapes overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Shape* const first, const Shape* const second);
	// Test if two shapes with given positions overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Shape* const first, const units::Coordinate2D& firstPos, const Shape* const second, const units::Coordinate2D& secondPos);
	// Test if two shapes with given positions overlap each other.
	// Gives the normal and distance that make up the minimum translation vector of separation to move the first shape out of the second shape.
	// Returns true if they overlap.
	bool performSAT(const Shape* const first, const units::Coordinate2D& firstPos, const Shape* const second, const units::Coordinate2D& secondPos,
	                units::Coordinate2D& out_norm, units::Coordinate& out_dist);
	// Describes the type of collision.
	enum class HybridResult {
		NONE, // No collision.
		MTV,  // Current collision.
		SWEEP // Future collision.
	};
	// Find when a collision will occur for one moving and one stationary shape, and the normal of their collision, if they collide.
	// Takes shapes, their positions, and the first shape's movement vector.
	// out_norm  - The collision normal for the first shape (reverse it for the second).
	// out_t     - For SWEEP results: a value in range [0,1], indicating when along the delta vector the collision occurs.
	//             For MTV results: The shapes are already colliding. Gives distance to travel along the norm to separate.
	// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
	HybridResult performHybridSAT(const Shape* const first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
		const Shape* const second, const units::Coordinate2D& secondPos, units::Coordinate2D& out_norm, units::Fraction& out_t);
	// Find when a collision will occur for two moving objects and the normal of their collision, if they collide.
	// Takes shapes, their positions, and movement vectors.
	// out_norm  - The collision normal for the first shape (reverse it for the second).
	// out_t     - For SWEEP results: a value in range [0,1], indicating when along the delta vectors the collision occurs.
	//             For MTV results: The shape are already colliding. Gives distance to travel along the norm to separate.
	// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
	HybridResult performHybridSAT(const Shape* const first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
		const Shape* const second, const units::Coordinate2D& secondPos, const units::Coordinate2D& secondDelta, units::Coordinate2D& out_norm, units::Fraction& out_t);

	// Given two shapes, find the axes of separation for them.
	// If given an unknown shape, converts the shape to a polygon.
	// Returns a vector of normalized separating axes.
	std::vector<units::Coordinate2D> getSeparatingAxes(const Shape* const first, const Shape* const second);
}

#endif // _SAT_H