#ifndef _INTERSECTION_MATH_H
#define _INTERSECTION_MATH_H

#include "../Units.h"

class LineSegment;
class Ray;
class Shape;
class Polygon;
class Rectangle;

namespace isect {
	// Intersections with points. No need to output a point of collision.
	bool intersects(const Rectangle& r, const units::Coordinate2D p);
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

	// Intersections with shapes. No output points of collision.
	bool intersects(const Rectangle& first, const Rectangle& second);
	bool intersects(const Rectangle& r, const LineSegment& l);
	// Uses SAT. Note that this will return false if the polygons "touch" but have no overlap.
	bool intersects(const Polygon& first, const Polygon& second);
	// Uses SAT. Note that this will return false if the shapes "touch" but have no overlap.
	bool intersects(const Shape& first, const Shape& second);
}

// Tests for if a colliding shape when moved by delta will collide with another shape.
namespace collision_math {
	// An epsilon value for how far to push colliding polygons out of collidee polygons.
	// Ensure this is large enough that adding it to the push-out distance will gurantee there is no longer a collision.
	// One tenth of a pixel seems like a good choice -- far enough that we should avoid all floating points issues; close
	// enough that it is invisible under most circumstances.
	const units::Coordinate COLLISION_PUSHOUT_DISTANCE = 0.1f;

	// Just check whether or not they collide. dir is the direction the collider is moving (normalized vector), and delta is by how much.
	bool collides(const Polygon& collider, const units::Coordinate2D& dir, const units::Coordinate delta, const Polygon& other);

	// Test for collision (returns true if there is one).
	// dir is the direction the collider is moving (normalized vector), and delta is by how much.
	// If there is a collision:
	// out_normal: the normal of the edge or vertex the collider ran into.
	// out_delta: how far the collider can move along delta before reaching the other polygon (how far it is safe to move along delta).
	bool collides(const Polygon& collider, const units::Coordinate2D& dir, units::Coordinate delta, const Polygon& other,
		units::Coordinate2D& out_normal, units::Coordinate& out_delta);
}

#endif // _INTERSECTION_MATH_H