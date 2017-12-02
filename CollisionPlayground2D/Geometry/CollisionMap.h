#pragma once
#ifndef _COLLISION_MAP_H
#define _COLLISION_MAP_H

#include <vector>

#include "Collidable.h"
#include "Units.h"

class CollisionMap {
public:
	virtual ~CollisionMap() {}
	// Given a collider and its delta, return a set of shapes it may collide with.
	virtual const std::vector<Collidable*> getColliding(const Collidable& collider, const units::Coordinate2D& delta) const = 0;
	// Given a collider, return a set of shapes it may overlap with.
	virtual const std::vector<Collidable*> getColliding(const Collidable& collider) const {
		return getColliding(collider, units::Coordinate2D(0, 0));
	}
};

#endif // _COLLISION_MAP_H