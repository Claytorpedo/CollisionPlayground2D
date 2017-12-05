#include "sat.h"

#include <vector>

#include "Units.h"
#include "DebugLogger.h"
#include "ShapeContainer.h"
#include "Shape.h"
#include "Rectangle.h"
#include "Polygon.h"
#include "Circle.h"

namespace geom {
	namespace sat {
		// System for finding the separating axes for the given shapes.
		// Determine the type of shape the first one is, then see if it forms a special case when paired with the second shape.
		// Returns true if it encounteres a special case that handled both shapes.
		inline bool _get_separating_axes(const ShapeContainer& first, const ShapeContainer& second, const Coord2& offset, std::vector<Coord2>& axes) {
			switch (first.type()) {
			case (ShapeType::RECTANGLE):
				axes.push_back(Coord2(1, 0)); // Rectangles are axis-alligned.
				axes.push_back(Coord2(0, 1));
				if (second.type() == ShapeType::RECTANGLE) // Rectangles will share axes.
					return true;
				break;
			case (ShapeType::POLYGON):
				axes.reserve(axes.size() + first.poly().size());
				for (std::size_t i = 0; i < first.poly().size(); ++i)
					axes.push_back(first.poly().getEdgeNorm(i));
				break;
			case (ShapeType::CIRCLE):
			{
				const Coord2 firstPos(first.circle().center + offset);
				if (second.type() == ShapeType::CIRCLE) { // Only one axis for two circles.
					const Coord2 axis = firstPos - second.circle().center;
					axes.push_back(axis.x == 0 && axis.y == 0 ? Coord2(0, 1) : axis.normalize());
					return true;
				}
				// Get axis from circle to the cloeset point/vertex on the other shape.
				const Coord2 axis = second.shape().getClosestTo(firstPos) - firstPos;
				if (axis.x != 0 || axis.y != 0) // If this is a zero vector, they are already overlapping, and can use other axes for the MTV.
					axes.push_back(axis.normalize());
			}
			break;
			default:
				DBG_WARN("Unhandled shape type for SAT. Converting to polygon.");
				Polygon p = first.shape().toPoly();
				for (std::size_t i = 0; i < p.size(); ++i)
					axes.push_back(p.getEdgeNorm(i));
			}
			return false;
		}
		// Gets the separating axes for two shapes.
		std::vector<Coord2> getSeparatingAxes(const ShapeContainer& first, const ShapeContainer& second, const Coord2& offset) {
			std::vector<Coord2> axes;
			if (_get_separating_axes(first, second, offset, axes))
				return axes;
			_get_separating_axes(second, first, -offset, axes);
			return axes;
		}
	}
}
