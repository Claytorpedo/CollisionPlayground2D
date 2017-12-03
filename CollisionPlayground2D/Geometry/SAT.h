#pragma once
#ifndef _GEOM_SAT_H
#define _GEOM_SAT_H

#include <vector>

#include "Units.h"

namespace geom {
	class ShapeContainer;

	// Separating Axis Theorem tests.
	// For all tests, "touching" shapes are not considered intersecting: they must overlap.
	namespace sat {
		// Test if two shapes overlap each other.
		// Returns true if they overlap.
		bool performSAT(const ShapeContainer& first, const ShapeContainer& second);
		// Test if two shapes with given positions overlap each other.
		// Returns true if they overlap.
		bool performSAT(const ShapeContainer& first, const Coord2& firstPos, const ShapeContainer& second, const Coord2& secondPos);
		// Test if two shapes with given positions overlap each other.
		// Gives the normal and distance that make up the minimum translation vector of separation to move the first shape out of the second shape.
		// Returns true if they overlap.
		bool performSAT(const ShapeContainer& first, const Coord2& firstPos, const ShapeContainer& second, const Coord2& secondPos,
			Coord2& out_norm, gFloat& out_dist);
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
		HybridResult performHybridSAT(const ShapeContainer& first, const Coord2& firstPos, const Coord2& firstDelta,
			const ShapeContainer& second, const Coord2& secondPos, Coord2& out_norm, gFloat& out_t);
		// Find when a collision will occur for two moving objects and the normal of their collision, if they collide.
		// Takes shapes, their positions, and movement vectors.
		// out_norm  - The collision normal for the first shape (reverse it for the second).
		// out_t     - For SWEEP results: a value in range [0,1], indicating when along the delta vectors the collision occurs.
		//             For MTV results: The shape are already colliding. Gives distance to travel along the norm to separate.
		// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
		HybridResult performHybridSAT(const ShapeContainer& first, const Coord2& firstPos, const Coord2& firstDelta,
			const ShapeContainer& second, const Coord2& secondPos, const Coord2& secondDelta, Coord2& out_norm, gFloat& out_t);

		// Given two shapes, find the axes of separation for them. Offset is first's position - second's position.
		// If given an unknown shape type, converts the shape to a polygon and uses that.
		// Returns a vector of normalized separating axes.
		std::vector<Coord2> getSeparatingAxes(const ShapeContainer& first, const ShapeContainer& second, const Coord2& offset = Coord2(0, 0));
	}
}

#endif // _GEOM_SAT_H
