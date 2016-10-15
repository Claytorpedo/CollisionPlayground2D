#include "Shape.h"

#include "Polygon.h"

#include <iostream>

units::Coordinate Shape::side(direction::Direction side) const {
	if (side == direction::LEFT)
		return left();
	if (side == direction::RIGHT)
		return right();
	if (side == direction::UP)
		return top();
	if (side == direction::DOWN)
		return bottom();
	std::cerr << "Error: Invalid direction to get side.\n";
	return 0;
}

Polygon Shape::extend(units::Coordinate2D delta) const {
	Polygon p(this->toPoly());
	return p.extend(delta);
}