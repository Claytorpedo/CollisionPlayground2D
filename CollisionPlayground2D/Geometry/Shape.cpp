#include "Shape.h"

#include <iostream>

#include "Units.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Projection.h"

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

Projection Shape::getProjection(const units::Coordinate2D& axis) const {
	Polygon p(this->toPoly());
	return p.getProjection(axis);
}

Polygon Shape::extend(const units::Coordinate2D& dir, const units::Coordinate dist) const {
	Polygon p(this->toPoly());
	return p.extend(dir, dist);
}

Rectangle Shape::getAABB() const {
	return Rectangle(left(), top(), right() - left(), bottom() - top());
}

units::Coordinate2D Shape::getClosestTo(const units::Coordinate2D& point) const {
	return this->toPoly().getClosestTo(point);
}