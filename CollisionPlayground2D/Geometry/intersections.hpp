#pragma once
#ifndef INCLUDE_GEOM_INTERSECTIONS_HPP
#define INCLUDE_GEOM_INTERSECTIONS_HPP

#include "units.hpp"

// See if geometry primatives intersect each other.
namespace geom {
	class LineSegment;
	class Ray;
	class ShapeContainer;
	class Rect;
	class Polygon;
	class Circle;

	// Intersections with points. No need to output a point of collision. -------------------------------------------------

	bool intersects(const Rect& r, const Coord2& p);
	bool intersects(const LineSegment& l, const Coord2& p);
	bool intersects(const Ray& r, const Coord2& p);

	// Intersection functions that return true/false, and do not find the specific point of collision. --------------------

	bool intersects(const LineSegment& a, const LineSegment& b);
	bool intersects_ignore_parallel(const Ray& r, const LineSegment& l); // Parallel lines are not considered intersecting.

	// Intersection functions that also output the point of collision, if a collision occurred. ----------------------------
	
	// In the case of collinear line segments, out_intersection is set to the closest point of overlap to a's start point.
	bool intersects(const LineSegment& a, const LineSegment& b, Coord2& out_intersection);
	// In the case of collinear line segments, out_t is set to the closest distance of overlap to the ray's origin.
	bool intersects(const Ray& r, const LineSegment& l, gFloat& out_t);
	// Parallel lines are not considered intersecting.
	bool intersects_ignore_parallel(const Ray& r, const LineSegment& l, gFloat& out_t);

	// Intersections of shapes with primatives -----------------------------------------------------------------------------

	// Intersection between a rectangle and a line segment. Considered intersecting if they touch.
	bool intersects(const Rect& r, const LineSegment& l);

	bool intersects(const Ray& ray, const Rect& rect);
	bool intersects(const Ray& ray, const Rect& rect, const Coord2& pos);
	bool intersects(const Ray& r, const Polygon& p, const Coord2& pos=Coord2(0, 0));
	bool intersects(const Ray& r, const Circle& circle, const Coord2& pos=Coord2(0, 0));
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos=Coord2(0,0));
}

#endif // INCLUDE_GEOM_INTERSECTIONS_HPP
