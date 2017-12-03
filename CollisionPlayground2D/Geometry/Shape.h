#pragma once
#ifndef _GEOM_SHAPE_H
#define _GEOM_SHAPE_H

#include "Units.h"
#include "Direction.h"

// Parent class for geometry objects.
namespace geom {
	class Polygon;
	class Rectangle;
	struct Projection;

	class Shape {
	public:
		virtual ~Shape() {}

		// Get leftmost x coordinate.
		virtual gFloat left()   const = 0;
		// Get rightmost x coordinate.
		virtual gFloat right()  const = 0;
		// Get topmost y coordinate.
		virtual gFloat top()    const = 0;
		// Get bottommost y coordinate.
		virtual gFloat bottom() const = 0;
		// Get the shape's projection along a given axis.
		// Default implementation uses the toPoly conversion.
		virtual Projection getProjection(const Coord2& axis) const;

		// Convert the shape into a polygon.
		// All child classes should be able to be at least approximated by a convex polygon.
		virtual Polygon toPoly() const = 0;
		// Extend a shape in a direction by a distance.
		virtual Polygon extend(const Coord2& dir, const gFloat dist) const;
		// Get the axis-aligned bounding box for a shape.
		virtual Rectangle getAABB() const;
		// Find the closest vertex/point to a given point.
		virtual Coord2 getClosestTo(const Coord2& point) const;

		// Get x or y coodrinate in a given direction.
		gFloat side(Direction side) const;
	};
}
#endif //GEOM_SHAPE_H