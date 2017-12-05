#pragma once
#ifndef _GEOM_INTERSECTIONS_H
#define _GEOM_INTERSECTIONS_H

#include "units.h"

// See if geometry primatives intersect each other.
namespace geom {
	class LineSegment;
	class Ray;
	class ShapeContainer;
	class Rect;

	// Intersections with points. No need to output a point of collision. -------------------------------------------------

	bool intersects(const Rect& r, const Coord2& p);
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

	// Intersection between a rectangle and a line segment. Considered intersecting if they touch.
	bool intersects(const Rect& r, const LineSegment& l);
}

#endif // _GEOM_INTERSECTIONS_H
