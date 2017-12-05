#pragma once
#ifndef _SAT_H
#define _SAT_H

#include <vector>

#include "units.h"

namespace geom {
	class ShapeContainer;
	namespace sat {
		// Given two shapes, find the axes of separation for them. Offset is first's position - second's position.
		// If given an unknown shape type, converts the shape to a polygon and uses that.
		// Returns a vector of normalized separating axes.
		std::vector<Coord2> getSeparatingAxes(const ShapeContainer& first, const ShapeContainer& second, const Coord2& offset = Coord2(0, 0));
	}
}

#endif // _SAT_H
