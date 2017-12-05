#pragma once
#ifndef _GEOM_MATH_H
#define _GEOM_MATH_H

#include "Units.h"
#include "Constants.h"

// Additional geometry math.
namespace geom {
	class Ray;

	namespace math {
		// Find the closest point on a line (made from a ray) to a given point.
		Coord2 closestPointOnLine(const Ray& ray, const Coord2& point);
		// Find the closest point on a line (made from two points) to a given point.
		Coord2 closestPointOnLine(const Coord2& line1, const Coord2& line2, const Coord2& point);

		// Check if a value is almost zero (if the given value is between positive and negative tolerance exclusive).
		inline bool almostZero(const gFloat t, const gFloat tolerance = constants::EPSILON) {
			return -tolerance < t && t < tolerance;
		}
		// Check if two values are almost the same (if their difference is less than the given tolerance).
		inline bool almostEquals(const gFloat a, const gFloat b, const gFloat tolerance = constants::EPSILON) {
			return std::abs(a - b) < tolerance;
		}
		// Check if two vectors' values are almost the same (if their difference is less than the given tolerance). Tolerance is applied separately to both values.
		inline bool almostEquals(const Coord2& a, const Coord2& b, const gFloat tolerance = constants::EPSILON) {
			return almostEquals(a.x, b.x, tolerance) && almostEquals(a.y, b.y, tolerance);
		}
		// Check if a value is between two bounds (inclusive).
		template<typename T>
		inline T clamp(const T val, const T bound_one, const T bound_two) {
			T min, max;
			if (bound_one <= bound_two) {
				min = bound_one;
				max = bound_two;
			} else {
				max = bound_two;
				min = bound_one;
			}
			return val < min ? min : val > max ? max : val;
		}
	}
}

#endif // _GEOM_MATH_H