#pragma once
#ifndef _SHAPE_H
#define _SHAPE_H

#include "../Units.h"
#include "../Constants.h"
#include "../Util.h"
#include "../Graphics.h"
#include "../Direction.h"

class Polygon;
class Rectangle;

// Parent class for geometry objects.

class Shape {
public:
	Shape(){}
	~Shape(){}

	// Get leftmost x coordinate.
	virtual units::Coordinate left()   const = 0;
	// Get rightmost x coordinate.
	virtual units::Coordinate right()  const = 0;
	// Get topmost y coordinate.
	virtual units::Coordinate top()    const = 0;
	// Get bottommost y coordinate.
	virtual units::Coordinate bottom() const = 0;

	// Convert the shape into a polygon.
	// All child classes should be able to be at least approximated by a convex polygon.
	virtual Polygon toPoly() const = 0;
	// Extend a shape in a direction by a distance.
	virtual Polygon extend(const units::Coordinate2D& dir, const units::Coordinate dist) const;
	// Get the axis-aligned bounding box for a shape.
	virtual Rectangle getAABB() const;

	// Get x or y coodrinate in a given direction.
	units::Coordinate side(direction::Direction side) const;
};

#endif //_SHAPE_H