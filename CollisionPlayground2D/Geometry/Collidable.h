#pragma once
#ifndef _COLLIDABLE_H
#define _COLLIDABLE_H

#include "../Units.h"
#include "Polygon.h"
#include "SAT.h"

class CollisionMap;

class Collidable {
public:
	// Keep a small space buffer around a polygon when moving towards it, to avoid moving into a currently-colliding state.
	// Acts as if making the polygon slightly larger.
	static const units::Coordinate COLLISION_BUFFER;
	// Minimum movement to consider when looking to see if the collider is stuck in a wedge (if moving more than this, considered not stuck).
	static const units::Coordinate WEDGE_MOVE_THRESH;
	// Number of attempts to resolve a situation where shapes are already overlapping.
	static const unsigned int COLLISION_DEBUG_MAX_ATTEMPTS;

	// Get the buffer amount to maintain to avoid moving to a collision state.
	static inline units::Coordinate getPushoutDistance(const units::Coordinate2D& travelDir, const units::Coordinate2D& collisionNormal) {
		// buffer_dist / cos(theta) = hypotenuse; cos(theta) = norm * dir (norm should be reversed, but we can just negate the end product).
		return -(COLLISION_BUFFER / collisionNormal.dot(travelDir));
	}
	enum class CollisionType {
		NONE,       // Collisions are ignored (noclip), or there was no collision.
		DEFLECTION, // Collisions result in deflection along edges.
		REVERSE,    // Collisions result in reversing direction.
		REFLECT,    // Collisions result in reflecting/bouncing off edges.
		_DEBUG_     // There is an error that must be resolved (the shapes are overlapping).
	};

	struct CollisionInfo {
		bool isCollision;                      // Whether a collision occurred.
		const Polygon* const collider;         // The collider for collision testing.
		const units::Coordinate2D originalDir; // Original direction of the delta vector.
		units::Coordinate2D currentDir;        // Direction the collider is currently travelling in.
		units::Coordinate remainingDist;       // Distance left for the collider to move.
		units::Coordinate moveDist;            // Distance collidable can move before a collision occurs.
		units::Coordinate2D currentPosition;   // The collider's current position.
		units::Coordinate2D normal;            // Collision normal. Not necessarily normalized.
		CollisionInfo(const Polygon* const collider, units::Coordinate2D position, units::Coordinate2D dir, units::Coordinate dist) :
			isCollision(false), collider(collider), originalDir(dir), currentDir(dir), remainingDist(dist),
			moveDist(0), currentPosition(position), normal(0,0) {}
	};
	Collidable() : type(CollisionType::DEFLECTION) {}
	Collidable(CollisionType type) : type(type) {}
	virtual ~Collidable() = 0;

	// Takes the collidable's origin, its bounding shape, the delta it is moving in, and the objects it can collide with.
	// Calls onCollision when collisions occur, if any special action is to be taken.
	// Returns the final position of the collider.
	units::Coordinate2D move(const units::Coordinate2D& origin, const Polygon& collider,
		                     const units::Coordinate2D& delta, const CollisionMap& collisionMap);
protected:
	CollisionType type;

	// What to do on collision. This can be used to handle special collisions.
	// Default implementation simply returns true, to continue the algorithm.
	// Return true if the algorithm should continue as normal, false if it should stop.
	virtual bool onCollision(CollisionInfo& info);

private:
	// Find the nearest collision from a list of polygons.
	sat::HybridResult _find_closest_collision(const CollisionMap& collisionMap, CollisionInfo& info) const;
	// Algorithm for deflecting-type collisions.
	void _move_deflection(CollisionInfo& info, const CollisionMap& collisionMap);
	// Attempt to fix currently-overlaping collisions.
	// Returns true if the situation is known to be resolved (collider is no-longer colliding). False indicates an unknown state (may or may not be resolved).
	bool _debug_collision(CollisionInfo& info, const CollisionMap& collisionMap);
};

#endif //_COLLIDABLE_H