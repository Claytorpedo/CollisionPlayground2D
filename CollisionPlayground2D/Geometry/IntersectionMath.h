#ifndef _INTERSECTION_MATH_H
#define _INTERSECTION_MATH_H

#include "../Units.h"

class LineSegment;
class Ray;
class Shape;
class Polygon;
class Rectangle;

namespace isect {
	// Intersections with points. No need to output a point of collision. -------------------------------------------------

	bool intersects(const Rectangle& r, const units::Coordinate2D p);
	bool intersects(const LineSegment& l, const units::Coordinate2D p);
	bool intersects(const Ray& r, const units::Coordinate2D p);

	// Intersection functions that return true/false, and do not find the specific point of collision. --------------------

	// These are optimized for speed, saving math by not calculating exactly where the collision occurred.
	bool intersects(const LineSegment& a, const LineSegment& b);

	// Intersection functions that also output the point of collision, if a collision occurred. ----------------------------
	
	// In the case of collinear line segments, out_intersection is set to the closest point of overlap to a's start point.
	bool intersects(const LineSegment& a, const LineSegment& b, units::Coordinate2D& out_intersection);
	// In the case of collinear line segments, out_intersection is set to the closest point of overlap to the ray's origin.
	bool intersects(const Ray& r, const LineSegment& l, units::Coordinate2D& out_intersection);

	// Intersections of shapes with primatives -----------------------------------------------------------------------------
	bool intersects(const Rectangle& r, const LineSegment& l);

	// Intersections with shapes. No output points of collision. -----------------------------------------------------------
	// Note that "touching" shapes are not considered intersecting if they have no overlap.

	bool intersects(const Rectangle& first, const Rectangle& second);
	bool intersects(const Polygon& first, const Polygon& second);
	bool intersects(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos);
	bool intersects(const Shape& first, const Shape& second);
}

#endif // _INTERSECTION_MATH_H