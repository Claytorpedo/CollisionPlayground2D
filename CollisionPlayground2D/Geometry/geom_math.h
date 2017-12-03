#pragma once
#ifndef _GEOM_MATH_H
#define _GEOM_MATH_H

#include "Units.h"

// Additional geometry math.
namespace geom {
	class Ray;

	// Find the closest point on a line (made from a ray) to a given point.
	Coord2 closestPointOnLine(const Ray& ray, const Coord2& point);
	// Find the closest point on a line (made from two points) to a given point.
	Coord2 closestPointOnLine(const Coord2& line1, const Coord2& line2, const Coord2& point);
}

#endif // _GEOM_MATH_H
