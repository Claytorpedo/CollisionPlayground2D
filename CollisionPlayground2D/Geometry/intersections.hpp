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
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos=Coord2(0, 0));
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos=Coord2(0,0));

	// Gets the first intersection for the ray and rectangle. If ray's origin intersects the rectangle, then out_t == 0.
	bool intersects(const Ray& ray, const Rect& rect, const Coord2& pos, gFloat& out_t);
	// Gets the first intersection along the ray between the ray and the polygon. (If ray's origin intersects the polygon, then out_t == 0.)
	//bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_t);
	// Gets the first intersection for the ray and circle. If the ray's origin intersects the circle, then out_t == 0.
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_t);
	// Gets the first intersection along the ray between the ray and the shape. (If ray's origin intersects the shape, then out_t == 0.)
	//bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_t);

	// Gets the first intersection and normal along the ray between the ray and the rectangle. (If ray's origin intersects the rectangle, then out_t == 0.)
	//bool intersects(const Ray& ray, const Rect& rect, const Coord2& pos, gFloat& out_t, Coord2& out_norm);
	// Gets the first intersection and normal along the ray between the ray and the polygon. (If ray's origin intersects the polygon, then out_t == 0.)
	//bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_t, Coord2& out_norm);
	// Gets the first intersection and normal for the ray and circle. If the ray's origin intersects the circle, then out_t == 0, and out_norm = (0,0).
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_t, Coord2& out_norm);
	// Gets the first intersection and normal along the ray between the ray and the shape. (If ray's origin intersects the shape, then out_t == 0.)
	//bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_t, Coord2& out_norm);

	// Get both intersections for the ray and rectangle. (If ray's origin intersects the rectangle, then out_enter == 0.)
	//bool intersects(const Ray& ray, const Rect& rect, const Coord2& pos, gFloat& out_enter, gFloat& out_exit);
	// Get both intersections for the ray and polygon. (If ray's origin intersects the polygon, then out_enter == 0.)
	//bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_enter, gFloat& out_exit);
	// Get both intersections for the ray and circle. (If ray's origin intersects the circle, then out_enter == 0.)
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_enter, gFloat& out_exit);
	// Get both intersections for the ray and shape. (If ray's origin intersects the shape, then out_enter == 0.)
	//bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_enter, gFloat& out_exit);

	// Get both intersections and normals for the ray and rectangle. (If ray's origin intersects the rectangle, then out_enter == 0.)
	//bool intersects(const Ray& ray, const Rect& rect, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit);
	// Get both intersections and normals for the ray and polygon. (If ray's origin intersects the polygon, then out_enter == 0.)
	//bool intersects(const Ray& r, const Polygon& p, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit);
	// Get both intersections and normals for the ray and circle. If the ray's origin intersects the circle, then out_enter == 0, and out_norm_enter = (0, 0).
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit);
	// Get both intersections and normals for the ray and shape. (If ray's origin intersects the shape, then out_enter == 0.)
	//bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit);
}

#endif // INCLUDE_GEOM_INTERSECTIONS_HPP
