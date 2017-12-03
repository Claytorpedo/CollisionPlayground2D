#pragma once
#ifndef _GEOM_WALL_H
#define _GEOM_WALL_H

#include "Collidable.h"
#include "ShapeContainer.h"

namespace geom {
	class Wall : public Collidable {
	public:
		Wall(const ShapeContainer& shape);
		Wall(const ShapeContainer& shape, const Coord2& position);
		~Wall();
		const Coord2& getPosition() const;
		const ShapeContainer& getCollider() const;
	private:
		ShapeContainer shape_;
		Coord2 position_;
	};
}
#endif // _GEOM_WALL_H
