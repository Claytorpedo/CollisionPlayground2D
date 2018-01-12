#include "gen.hpp"

namespace game {
	using namespace geom;

	Polygon genPoly(std::mt19937& rando, const Rect& region, const gFloat minRad, const gFloat maxRad, const std::size_t minVerts, const std::size_t maxVerts) {
		// Randomly generate the polygon's center within the region.
		std::uniform_real_distribution<gFloat> distX(region.left(), region.right());
		std::uniform_real_distribution<gFloat> distY(region.top(), region.bottom());
		const Coord2 cen(distX(rando), distY(rando));

		if (minVerts < 3 || maxVerts < 3) std::cerr << "Error: Cannot generate a polygon with fewer than 3 vertices. Defaulting to 3 minimum.\n";
		const std::size_t min = minVerts < 3 ? 3 : minVerts;
		std::uniform_int_distribution<std::size_t> distVerts(min, maxVerts < min ? min : maxVerts);
		const std::size_t numVerts(distVerts(rando));

		// Generate random numbers between 0 and tau (2pi) to make points around a circle.
		std::uniform_real_distribution<gFloat> distPI(0.0f, constants::TAU);
		std::vector<gFloat> piVec;
		piVec.reserve(numVerts);
		for (std::size_t i = 0; i < numVerts; ++i)
			piVec.push_back(distPI(rando));
		// Sort descending (so we have counterclockwise winding).
		std::sort(piVec.begin(), piVec.end(), [](const gFloat& lhs, const gFloat& rhs) {
			return lhs > rhs;
		});

		// Get radius for polygon.
		std::uniform_real_distribution<gFloat> distRad(minRad, maxRad);
		gFloat radius(distRad(rando));

		std::vector<Coord2> vertices;
		vertices.reserve(numVerts);
		for (std::size_t i = 0; i < numVerts; ++i)
			vertices.push_back(cen + Coord2(radius * std::cos(piVec[i]), radius * std::sin(piVec[i])));
		return Polygon(vertices);
	}
}