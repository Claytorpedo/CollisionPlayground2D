#include "Circle.h"

#include <vector>

#include "Polygon.h"
#include "../Units.h"
#include "../Constants.h"

const std::size_t Circle::SEGS_IN_POLY = 20;

Projection Circle::getProjection(const units::Coordinate2D& axis) const {
	units::Coordinate proj(axis.dot(center));
	return Projection(proj - radius, proj + radius);
}

Polygon Circle::toPoly() const {
	// Approximate a circle with line segments.
	std::vector<units::Coordinate2D> vertices;
	vertices.reserve(SEGS_IN_POLY);
	const units::Coordinate segSize(constants::TAU / static_cast<units::Coordinate>(SEGS_IN_POLY));
	for (std::size_t i = SEGS_IN_POLY; i > 0; --i) { // Wind counter-clockwise.
		const units::Coordinate theta(segSize * static_cast<units::Coordinate>(i));
		const units::Coordinate2D pos(radius * cos(theta), radius * sin(theta));
		vertices.push_back(center + pos);
	}
	return Polygon(vertices);
}