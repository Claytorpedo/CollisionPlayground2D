#ifndef _COLLISION_MATH_H
#define _COLLISION_MATH_H

#include "../Units.h"

class LineSegment;
class Ray;

namespace collision_math {
	// Intersections with points. No need to output a point of collision.
	bool intersects(const LineSegment& l, const units::Coordinate2D p);
	bool intersects(const Ray& r, const units::Coordinate2D p);

	// Intersection functions that return true/false, and do not find the specific point of collision.
	// These are optimized for speed, saving math by not calculating exactly where the collision occurred.
	bool intersects(const LineSegment& a, const LineSegment& b);

	// Intersection functions that also output the point of collision, if a collision occurred.
	// In cases where an intersection overlaps (say, collinear lines), out_intersection is set to the "closest"
	// point to the first parameter -- for example the closest point to a line segment's starting point.
	bool intersects(const LineSegment& a, const LineSegment& b, units::Coordinate2D& out_intersection);
	bool intersects(const Ray& r, const LineSegment& l, units::Coordinate2D& out_intersection);
}

#endif // _COLLISION_MATH_H