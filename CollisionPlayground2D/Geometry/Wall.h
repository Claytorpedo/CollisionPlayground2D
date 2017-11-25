#pragma once
#ifndef _WALL_H
#define _WALL_H

#include "Collidable.h"
#include "ShapeContainer.h"

class Wall : public Collidable {
public:
	Wall(const ShapeContainer& shape);
	Wall(const ShapeContainer& shape, const units::Coordinate2D& position);
	~Wall();
	const units::Coordinate2D& getPosition() const;
	const ShapeContainer& getCollider() const;
private:
	ShapeContainer shape_;
	units::Coordinate2D position_;
};

#endif // _WALL_H
