#ifndef _COLLISION_MATH_H
#define _COLLISION_MATH_H

#include "../Units.h"

class Polygon;

// Tests for if a colliding shape when moved by delta will collide with another shape.
namespace collision_math {
	// An epsilon value for how far to push colliding polygons out of collidee polygons.
	// Ensure this is large enough that adding it to the push-out distance will gurantee there is no longer a collision.
	// One tenth of a pixel seems like a good choice -- far enough that we should avoid all floating points issues; close
	// enough that it is invisible under most circumstances.
	const units::Coordinate COLLISION_PUSHOUT_DISTANCE = 0.1f;

	// Test for collision. dir is the direction the collider is moving (normalized vector), and dist is by how much.
	bool collides(const Polygon& collider, const units::Coordinate2D& dir, const units::Coordinate dist, const Polygon& other);
	// Test for collision. The colliding polygon must already be clip-extended by the delta vector (dir*dist).
	bool clippedCollides(const Polygon& clippedCollider, const Polygon& other);

	// Test for collision (returns true if there is one).
	// dir is the direction the collider is moving (normalized vector), and dist is by how much (delta = dir*dist).
	// If there is a collision:
	// out_dist: how far the collider can move along dir before reaching the other polygon (how far it is safe to move along dir).
	// out_norm: The collision normal.
	bool collides(const Polygon& collider, const units::Coordinate2D& dir, units::Coordinate dist, const Polygon& other,
		units::Coordinate& out_dist, units::Coordinate2D& out_norm);
	// Test for collision (returns true if there is one).
	// This function assumes that the collider has already been clip-extended by the delta vector (dir*dist).
	// dir is the direction the collider is moving (normalized vector), and dist is by how much (delta = dir*dist).
	// If there is a collision:
	// out_dist: how far the collider can move along dir before reaching the other polygon (how far it is safe to move along dir).
	// out_norm: The collision normal.
	bool clippedCollides(const Polygon& clippedCollider, const units::Coordinate2D& dir, units::Coordinate dist, const Polygon& other,
		units::Coordinate& out_dist, units::Coordinate2D& out_norm);
}

#endif // _COLLISION_MATH_H