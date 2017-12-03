#pragma once
#ifndef _GEOM_UTIL_H_
#define _GEOM_UTIL_H_

#include <cmath>

#include "Units.h"
#include "Constants.h"

namespace geom {
	namespace util {
		template<typename T>
		inline bool almostZero(const T t, const T tolerance = constants::EPSILON) { // Use for floating point numbers.
			return -tolerance < t && t < tolerance;
		}
		inline bool almostEquals(const gFloat a, const gFloat b, const gFloat tolerance = constants::EPSILON) {
			return std::abs(a - b) < tolerance;
		}
		inline bool almostEquals(const Coord2& a, const Coord2& b, const gFloat tolerance = constants::EPSILON) {
			return almostEquals(a.x, b.x, tolerance) && almostEquals(a.y, b.y, tolerance);
		}
		// Check if a value is between two bounds.
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

#endif // _GEOM_UTIL_H_