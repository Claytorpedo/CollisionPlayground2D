#pragma once
#ifndef _DEFINITIONS_H
#define _DEFINITIONS_H

#include "catch.hpp"

#include <vector>

#include "../Constants.h"
#include "../Units.h"
#include "../Geometry/Movable.h"

#define ApproxEps(x) Approx(x).margin(constants::EPSILON)
#define ApproxCollides(x) Approx(x).margin(Movable::COLLISION_BUFFER) // For collision resolution, where perfect accuracy isn't as important.

using namespace units;

namespace shapes { // Some shapes we'll use in various tests.
	const std::vector<Coordinate2D> rightTri = { Coordinate2D(0, 0), Coordinate2D(1, 1), Coordinate2D(1, 0) };    // A right angle triangle.
	const std::vector<Coordinate2D> tri =      { Coordinate2D(-1, -2), Coordinate2D(1, 0), Coordinate2D(3, -1) }; // A regular/scalene triangle.
	const std::vector<Coordinate2D> edgeTri =  { Coordinate2D(-1, -2), Coordinate2D(-1, 0), Coordinate2D(1, 0) }; // Shares an edge with tri.
	const std::vector<Coordinate2D> edgeTriR = { Coordinate2D(0, 0), Coordinate2D(0, 1), Coordinate2D(1, 1) };    // Shares an edge with rightTri.
	const std::vector<Coordinate2D> isoTri =   { Coordinate2D(0, 0), Coordinate2D(1, 1), Coordinate2D(2, 0) };    // An isosceles triangle.
	const std::vector<Coordinate2D> octagon =  { Coordinate2D(0,2), Coordinate2D(1.5f,1.5f), Coordinate2D(2,0), Coordinate2D(1.5f,-1.5f),
	                                             Coordinate2D(0,-2), Coordinate2D(-1.5f,-1.5f), Coordinate2D(-2,0), Coordinate2D(-1.5f,1.5f) };
	// An arbitrary polygon.
	const std::vector<Coordinate2D> arb =      { Coordinate2D(0,0), Coordinate2D(1,2), Coordinate2D(2,2), Coordinate2D(3,1), Coordinate2D(3, -1), Coordinate2D(1, -2) };
}

namespace separating_axes {
	const std::size_t RECT_NUM_AXES = 2; // Rectangles are axis-alligned.
}

#endif // _DEFINITIONS_H