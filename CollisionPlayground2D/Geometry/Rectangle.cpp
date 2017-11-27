#include "Rectangle.h"
#include "../Constants.h"

inline bool Rectangle::isInside(const Rectangle& o) const {
	return right() <= o.right() + constants::EPSILON && bottom() <= o.bottom() + constants::EPSILON &&
	       left() >= o.left() - constants::EPSILON   && top() >= o.top() - constants::EPSILON;
}

Projection Rectangle::getProjection(const units::Coordinate2D& axis) const {
	units::Coordinate proj(axis.dot(topLeft()));
	units::Coordinate min(proj), max(proj);
	proj = axis.dot(topRight());
	if (proj < min)
		min = proj;
	else
		max = proj;
	proj = axis.dot(bottomLeft());
	if (proj < min)
		min = proj;
	else if (proj > max)
		max = proj;
	proj = axis.dot(bottomRight());
	if (proj < min)
		min = proj;
	else if (proj > max)
		max = proj;
	return Projection(min, max);
}

Polygon Rectangle::toPoly() const {
	std::vector<units::Coordinate2D> vertices;
	vertices.reserve(4);
	vertices.push_back(topLeft());
	vertices.push_back(bottomLeft());
	vertices.push_back(bottomRight());
	vertices.push_back(topRight());
	return Polygon(vertices);
}