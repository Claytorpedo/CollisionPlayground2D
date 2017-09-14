#pragma once
#ifndef _COLLIDABLE_H
#define _COLLIDABLE_H

#include "../Units.h"
#include "Polygon.h"

class CollisionMap;

class Collidable {
public:
	enum CollisionType {
		DEFLECTION, // Collisions result in deflection along edges.
		REVERSE,    // Collisions result in reversing direction.
		REFLECT     // Collisions result in reflecting/bouncing off edges.
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
	Collidable() : type(DEFLECTION) {}
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
	void find_closest_collision(const Polygon& collider, const CollisionMap& collisionMap, CollisionInfo& info) const;

	void move_deflection(CollisionInfo& info, const CollisionMap& collisionMap);
};

#endif //_COLLIDABLE_H