#pragma once
#ifndef _CIRCLE_H
#define _CIRCLE_H

#include "Shape.h"
#include "Projection.h"
#include "../Units.h"

class Circle : public Shape {
public:
	static const std::size_t SEGS_IN_POLY;

	units::Coordinate2D center;
	units::Coordinate radius;

	Circle() : center(0, 0), radius(0) {}
	Circle(units::Coordinate radius) : center(0, 0), radius(radius) {}
	Circle(units::Coordinate center_x, units::Coordinate center_y, units::Coordinate radius) : center(center_x, center_y), radius(radius) {}
	Circle(units::Coordinate2D center, units::Coordinate radius) : center(center), radius(radius) {}

	inline virtual units::Coordinate left()   const { return center.x - radius; }
	inline virtual units::Coordinate right()  const { return center.x + radius; }
	inline virtual units::Coordinate top()    const { return center.y - radius; }
	inline virtual units::Coordinate bottom() const { return center.y + radius; }

	virtual Projection getProjection(const units::Coordinate2D& axis) const;
	virtual Polygon toPoly() const;
};

#endif // _CIRCLE_H
