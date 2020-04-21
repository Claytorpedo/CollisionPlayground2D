#include "generator.hpp"

#include <algorithm>
#include <iostream>

namespace gen {
void init() {
	rng.seed(std::random_device{}());
}

ctp::Polygon poly(const ctp::gFloat minRad, const ctp::gFloat maxRad, const std::size_t minVerts, const std::size_t maxVerts) {
	if (minVerts < 3 || maxVerts < 3) std::cerr << "Error: Cannot generate a polygon with fewer than 3 vertices. Defaulting to 3 minimum.\n";
	const std::size_t min = minVerts < 3 ? 3 : minVerts;
	std::uniform_int_distribution<std::size_t> distVerts(min, maxVerts < min ? min : maxVerts);
	const std::size_t numVerts(distVerts(rng));

	// Generate random numbers between 0 and tau (2pi) to make points around a circle.
	std::uniform_real_distribution<ctp::gFloat> distPI(0.0f, ctp::constants::TAU);
	std::vector<ctp::gFloat> piVec;
	piVec.reserve(numVerts);
	for (std::size_t i = 0; i < numVerts; ++i)
		piVec.push_back(distPI(rng));
	// Sort descending (so we have counterclockwise winding).
	std::sort(piVec.begin(), piVec.end(), [](const ctp::gFloat& lhs, const ctp::gFloat& rhs) {
		return lhs > rhs;
		});

	// Get radius for polygon.
	std::uniform_real_distribution<ctp::gFloat> distRad(minRad, maxRad);
	ctp::gFloat radius(distRad(rng));

	std::vector<ctp::Coord2> vertices;
	vertices.reserve(numVerts);
	for (std::size_t i = 0; i < numVerts; ++i)
		vertices.push_back(ctp::Coord2(radius * std::cos(piVec[i]), radius * std::sin(piVec[i])));
	return ctp::Polygon(vertices);
}
ctp::Coord2 coord2(const ctp::Rect& region) {
	std::uniform_real_distribution<ctp::gFloat> X(region.left(), region.right());
	std::uniform_real_distribution<ctp::gFloat> Y(region.top(), region.bottom());
	return ctp::Coord2(X(rng), Y(rng));
}
ctp::gFloat gFloat(const ctp::gFloat min, const ctp::gFloat max) {
	std::uniform_real_distribution<ctp::gFloat> f(min, max);
	return f(rng);
}
}
