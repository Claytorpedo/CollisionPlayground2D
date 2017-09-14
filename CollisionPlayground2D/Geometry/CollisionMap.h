#pragma once
#ifndef _COLLISION_MAP_H
#define _COLLISION_MAP_H

#include <vector>

#include "Shape.h"
#include "../Units.h"

class CollisionMap {
public:
	CollisionMap() {}
	~CollisionMap() {}
	// Given a collider and its delta, return a set of shapes it may collide with.
	virtual std::vector<Polygon> getColliding(const Shape& collider, const units::Coordinate2D& delta) const = 0;
};

#endif // _COLLISION_MAP_H