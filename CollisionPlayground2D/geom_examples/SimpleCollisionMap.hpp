#pragma once
#ifndef INCLUDE_GAME_SIMPLE_COLLISION_MAP_HPP
#define INCLUDE_GAME_SIMPLE_COLLISION_MAP_HPP

#include <Geometry2D/Geometry.hpp>

// Extremely simple CollisionMap implementation: no data structure speedup at all.

namespace game {
	class SimpleCollisionMap : public geom::CollisionMap {
	public:
		std::vector<geom::Collidable*> obstacles;
		SimpleCollisionMap() {}
		~SimpleCollisionMap() {
			clear();
		}
		virtual const std::vector<geom::Collidable*> getColliding(const geom::Collidable& collider, const geom::Coord2& delta) const {
			return std::vector<geom::Collidable*>(obstacles.begin(), obstacles.end());
		}
		void add(geom::Collidable* collidable) {
			obstacles.push_back(collidable);
		}
		inline geom::Collidable* operator[](std::size_t index) const {
			return obstacles[index];
		}
		inline std::size_t size() const {
			return obstacles.size();
		}
		void clear() {
			for (std::size_t i = 0; i < obstacles.size(); ++i)
				delete obstacles[i];
			obstacles.clear();
		}
	};
}

#endif // INCLUDE_GAME_SIMPLE_COLLISION_MAP_HPP
