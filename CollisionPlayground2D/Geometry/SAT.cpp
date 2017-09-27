#include "SAT.h"

#include "../Units.h"
#include "../Constants.h"
#include "Polygon.h"

namespace {
	struct Projection {
		units::Coordinate min, max;
		Projection() : min(0), max(0) {}
		Projection(units::Coordinate min, units::Coordinate max) : min(min), max(max) {}
		inline Projection& operator+=(const units::Coordinate& v) { min += v; max += v; return *this; }
	};
	inline Projection _get_projection(const Polygon& poly, const units::Coordinate2D& axis) {
		units::Coordinate min(poly[0].dot(axis));
		units::Coordinate max(min);
		units::Coordinate proj;
		for (std::size_t i = 1; i < poly.size(); ++i) {
			proj = poly[i].dot(axis);
			if (proj < min)
				min = proj;
			else if (proj > max)
				max = proj;
		}
		return Projection(min, max);
	}
}

// Tests the axes of one polygon against the other using SAT.
inline bool _SAT(const Polygon& first, const Polygon& second) {
	const std::size_t size = first.size();
	units::Coordinate2D axis;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < size; ++i) {
		axis = first.getEdgeNorm(i); // Axis to project along.
		projFirst = _get_projection(first, axis);
		projSecond = _get_projection(second, axis);
		if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
			return false;
	}
	return true;
}

// Tests the axes of one polygon against the other using SAT. Offset is the position of polygon first - second.
inline bool _SAT(const Polygon& first, const Polygon& second, const units::Coordinate2D& offset) {
	const std::size_t size = first.size();
	units::Coordinate2D axis;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < size; ++i) {
		axis = first.getEdgeNorm(i); // Axis to project along.
		projFirst = _get_projection(first, axis);
		projSecond = _get_projection(second, axis);
		projFirst += offset.dot(axis); // Apply offset between the two polygons' positions.
		if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
			return false;
	}
	return true;
}

// Tests the axes of one polygon against the other using SAT. Offset is the position of polygon first - second.
// Outputs the normal and distance that make up the minimum translation vector.
inline bool _SAT(const Polygon& first, const Polygon& second, const units::Coordinate2D& offset, units::Coordinate2D& out_norm, units::Coordinate& out_dist) {
	const std::size_t size = first.size();
	units::Coordinate2D axis, norm(0, 0);
	units::Coordinate overlap1, overlap2, minDist(-1), testDist;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < size; ++i) {
		axis = first.getEdgeNorm(i); // Axis to project along.
		projFirst = _get_projection(first, axis);
		projSecond = _get_projection(second, axis);
		projFirst += offset.dot(axis); // Apply offset between the two polygons' positions.
		overlap1 = projFirst.max - projSecond.min;
		overlap2 = projSecond.max - projFirst.min;
		if (overlap1 < constants::EPSILON || overlap2 < constants::EPSILON)
			return false;
		// Find separation for this axis (assumes pushing out the first polygon).
		if (projFirst.min < projSecond.min) {
			testDist = overlap1;
			axis = -axis; // Ensure right direction to pushout the first polygon.
		} else {
			testDist = overlap2;
		}
		if (minDist == -1 || testDist < minDist) {
			minDist = testDist;
			norm = axis;
		}
	}
	out_norm = norm;
	out_dist = minDist;
	return true;
}

bool sat::performSAT(const Polygon& first, const Polygon& second) {
	return _SAT(first, second) && _SAT(second, first);
}

bool sat::performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos) {
	return _SAT(first, second, firstPos - secondPos) && _SAT(second, first, secondPos - firstPos);
}

bool sat::performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos,
                     units::Coordinate2D& out_norm, units::Coordinate& out_dist) {
	units::Coordinate2D norm1(0, 0), norm2(0, 0);
	units::Coordinate dist1(-1), dist2(-1);
	if (!_SAT(first, second, firstPos - secondPos, norm1, dist1))
		return false;
	if (!_SAT(second, first, secondPos - firstPos, norm2, dist2))
		return false;
	if (dist1 < dist2) {
		out_norm = norm1;
		out_dist = dist1;
	} else {
		out_norm = -norm2; // norm2 is relative to second polygon; reverse it.
		out_dist = dist2;
	}
	return true;
}