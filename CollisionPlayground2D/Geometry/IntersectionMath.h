#pragma once
#ifndef _GEOM_INTERSECTION_MATH_H
#define _GEOM_INTERSECTION_MATH_H

#include "Units.h"

namespace geom {
	class LineSegment;
	class Ray;
	class ShapeContainer;
	class Rectangle;

	// Intersections with points. No need to output a point of collision. -------------------------------------------------

	bool intersects(const Rectangle& r, const Coord2& p);
	bool intersects(const LineSegment& l, const Coord2& p);
	bool intersects(const Ray& r, const Coord2& p);

	// Intersection functions that return true/false, and do not find the specific point of collision. --------------------

	// These are optimized for speed, saving math by not calculating exactly where the collision occurred.
	bool intersects(const LineSegment& a, const LineSegment& b);

	// Intersection functions that also output the point of collision, if a collision occurred. ----------------------------
	
	// In the case of collinear line segments, out_intersection is set to the closest point of overlap to a's start point.
	bool intersects(const LineSegment& a, const LineSegment& b, Coord2& out_intersection);
	// In the case of collinear line segments, out_intersection is set to the closest point of overlap to the ray's origin.
	bool intersects(const Ray& r, const LineSegment& l, Coord2& out_intersection);

	// Intersections of shapes with primatives -----------------------------------------------------------------------------
	bool intersects(const Rectangle& r, const LineSegment& l);

	// Intersections with shapes. No output points of collision. -----------------------------------------------------------
	// Note that "touching" shapes are not considered intersecting if they have no overlap.

	bool intersects(const Rectangle& first, const Rectangle& second);
	bool intersects(const ShapeContainer& first, const Coord2& firstPos, const ShapeContainer& second, const Coord2& secondPos);
	bool intersects(const ShapeContainer& first, const ShapeContainer& second);
}

#endif // _GEOM_INTERSECTION_MATH_H