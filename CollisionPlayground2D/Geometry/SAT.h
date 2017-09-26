#pragma once
#ifndef _SAT_H
#define _SAT_H

#include "../Units.h"

class Polygon;

// Separating Axis Theorem tests.
// For all tests, "touching" polygons are not considered intersecting: they must overlap.
namespace sat {
	// Test if two convex polygons overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Polygon& first, const Polygon& second);
	// Test if two convex polygons with given positions overlap each other.
	// Returns true if they overlap.
	bool performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos);
	// Test if two convex polygons with given positions overlap each other.
	// Gives the normal and distance that make up the minimum translation vector of separation to move the first polygon out of the second polygon.
	// Returns true if they overlap.
	bool performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos,
	                units::Coordinate2D& out_norm, units::Coordinate& out_dist);
}

#endif // _SAT_H