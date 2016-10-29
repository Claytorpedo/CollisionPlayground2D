#ifndef _COLLISION_MATH_H
#define _COLLISION_MATH_H

#include "../Units.h"

class LineSegment;
class Ray;
class Shape;
class Polygon;
class Rectangle;

// Tests for if a colliding shape when moved by delta will collide with another shape.

namespace collision_math {
	// Just check whether or not they collide.
	bool collides(const Polygon& collider, const units::Coordinate2D delta, const Polygon& other);

	// Test for collision, and if so find the collision normal and depth/amount of overlap.
	bool collides(const Polygon& collider, const units::Coordinate2D delta, const Polygon& other,
		units::Coordinate2D& out_normal, units::Coordinate2D& out_depth);
}

#endif // _INTERSECTION_MATH_H