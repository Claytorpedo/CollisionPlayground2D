#pragma once
#ifndef _GEOM_COLLISION_MAP_H
#define _GEOM_COLLISION_MAP_H

#include <vector>

#include "Collidable.h"
#include "Units.h"

namespace geom {
	class CollisionMap {
	public:
		virtual ~CollisionMap() {}
		// Given a collider and its delta, return a set of shapes it may collide with.
		virtual const std::vector<Collidable*> getColliding(const Collidable& collider, const Coord2& delta) const = 0;
		// Given a collider, return a set of shapes it may overlap with.
		virtual const std::vector<Collidable*> getColliding(const Collidable& collider) const {
			return getColliding(collider, Coord2(0, 0));
		}
	};
}
#endif // _GEOM_COLLISION_MAP_H