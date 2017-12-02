#pragma once
#ifndef _GEOM_MATH_H
#define _GEOM_MATH_H

#include "Units.h"

class Ray;

// Additional geometry math.
namespace geom_math {
	// Find the closest point on a line (made from a ray) to a given point.
	units::Coordinate2D closestPointOnLine(const Ray& ray, const units::Coordinate2D& point);
	// Find the closest point on a line (made from two points) to a given point.
	units::Coordinate2D closestPointOnLine(const units::Coordinate2D& line1, const units::Coordinate2D& line2, const units::Coordinate2D& point);
}

#endif // _GEOM_MATH_H
