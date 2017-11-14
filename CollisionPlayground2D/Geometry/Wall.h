#pragma once
#ifndef _WALL_H
#define _WALL_H

#include "Collidable.h"
#include "Shape.h"

class Wall : public Collidable {
public:
	Wall(Shape* shape);
	Wall(Shape* shape, const units::Coordinate2D& position);
	~Wall();
	const units::Coordinate2D& getPosition() const;
	const Shape* const getCollider() const;
private:
	Shape* shape_;
	units::Coordinate2D position_;
};

#endif // _WALL_H
