#pragma once
#ifndef _SAT_H
#define _SAT_H

#include "../Units.h"

class Polygon;

// Separating Axis Theorem tests.
namespace sat {
	// Test if two convex polygons intersect each other.
	// "Touching" polygons are not considered intersecting: they must overlap.
	// Returns true if they intersect.
	bool performSAT(const Polygon& first, const Polygon& second);
	// Test if two convex polygons with given positions intersect each other.
	// "Touching" polygons are not considered intersecting: they must overlap.
	// Returns true if they intersect.
	bool performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos);
}

#endif // _SAT_H