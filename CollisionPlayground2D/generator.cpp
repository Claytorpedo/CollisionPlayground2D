#include "generator.hpp"

#include <algorithm>
#include <iostream>

namespace gen {
	void init() {
		rng.seed(std::random_device{}());
	}

	geom::Polygon poly(const geom::gFloat minRad, const geom::gFloat maxRad, const std::size_t minVerts, const std::size_t maxVerts) {
		if (minVerts < 3 || maxVerts < 3) std::cerr << "Error: Cannot generate a polygon with fewer than 3 vertices. Defaulting to 3 minimum.\n";
		const std::size_t min = minVerts < 3 ? 3 : minVerts;
		std::uniform_int_distribution<std::size_t> distVerts(min, maxVerts < min ? min : maxVerts);
		const std::size_t numVerts(distVerts(rng));

		// Generate random numbers between 0 and tau (2pi) to make points around a circle.
		std::uniform_real_distribution<geom::gFloat> distPI(0.0f, geom::constants::TAU);
		std::vector<geom::gFloat> piVec;
		piVec.reserve(numVerts);
		for (std::size_t i = 0; i < numVerts; ++i)
			piVec.push_back(distPI(rng));
		// Sort descending (so we have counterclockwise winding).
		std::sort(piVec.begin(), piVec.end(), [](const geom::gFloat& lhs, const geom::gFloat& rhs) {
			return lhs > rhs;
		});

		// Get radius for polygon.
		std::uniform_real_distribution<geom::gFloat> distRad(minRad, maxRad);
		geom::gFloat radius(distRad(rng));

		std::vector<geom::Coord2> vertices;
		vertices.reserve(numVerts);
		for (std::size_t i = 0; i < numVerts; ++i)
			vertices.push_back(geom::Coord2(radius * std::cos(piVec[i]), radius * std::sin(piVec[i])));
		return geom::Polygon(vertices);
	}
	geom::Coord2 coord2(const geom::Rect& region) {
		std::uniform_real_distribution<geom::gFloat> X(region.left(), region.right());
		std::uniform_real_distribution<geom::gFloat> Y(region.top(), region.bottom());
		return geom::Coord2(X(rng), Y(rng));
	}
}