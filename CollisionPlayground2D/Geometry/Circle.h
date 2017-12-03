#pragma once
#ifndef _CIRCLE_H
#define _CIRCLE_H

#include "Shape.h"
#include "Projection.h"
#include "Units.h"

namespace geom {
	class Circle : public Shape {
	public:
		static const std::size_t SEGS_IN_POLY;

		Coord2 center;
		gFloat radius;

		Circle() : center(0, 0), radius(0) {}
		Circle(gFloat radius) : center(0, 0), radius(radius) {}
		Circle(gFloat center_x, gFloat center_y, gFloat radius) : center(center_x, center_y), radius(radius) {}
		Circle(Coord2 center, gFloat radius) : center(center), radius(radius) {}

		inline virtual gFloat left()   const { return center.x - radius; }
		inline virtual gFloat right()  const { return center.x + radius; }
		inline virtual gFloat top()    const { return center.y - radius; }
		inline virtual gFloat bottom() const { return center.y + radius; }

		virtual Projection getProjection(const Coord2& axis) const;
		virtual Coord2 getClosestTo(const Coord2& point) const; // Gets closest point on the circle.
		virtual Polygon toPoly() const;
	};
}
#endif // _CIRCLE_H
