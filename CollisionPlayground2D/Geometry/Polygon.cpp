#include "Polygon.h"

#include <iostream>
#include <cmath>
#include <algorithm>

#include "Rectangle.h"
#include "Projection.h"
#include "../Units.h"
#include "../Constants.h"

const units::Coordinate Polygon::EPSILON_DEGREE_TOLERANCE = 0.00001f;

Polygon::Polygon() : vertices_(), x_min_(), x_max_(), y_min_(), y_max_() {}
Polygon::Polygon(std::vector<units::Coordinate2D> vertices) : vertices_(vertices), edge_normals_(vertices.size(), units::Coordinate2D(0,0)), x_min_(), x_max_(), y_min_(), y_max_() {
	_find_bounds();
}
Polygon::Polygon(std::vector<units::Coordinate2D> vertices, std::vector<units::Coordinate2D> edgeNormals)
	             : vertices_(vertices), edge_normals_(edgeNormals), x_min_(), x_max_(), y_min_(), y_max_() {
	_find_bounds();
}

void Polygon::_find_bounds() {
	if (vertices_.empty()) {
		x_min_ = 0; x_max_ = 0; y_min_ = 0; y_max_ = 0;
		return;
	}
	x_min_ = vertices_[0].x; x_max_ = vertices_[0].x;
	y_min_ = vertices_[0].y; y_max_ = vertices_[0].y;
	for (std::size_t i = 1; i < vertices_.size(); ++i) {
		if (x_min_ > vertices_[i].x)
			x_min_ = vertices_[i].x;
		if (x_max_ < vertices_[i].x)
			x_max_ = vertices_[i].x;
		if (y_min_ > vertices_[i].y) 
			y_min_ = vertices_[i].y;
		if (y_max_ < vertices_[i].y)
			y_max_ = vertices_[i].y;
	}
}

Polygon Polygon::generate(std::mt19937& rando, const Rectangle& region,
	const units::Coordinate minRad, const units::Coordinate maxRad, const std::size_t minVerts, const std::size_t maxVerts) {
	// Randomly generate the polygon's center within the region.
	std::uniform_real_distribution<units::Coordinate> distX(region.left(), region.right());
	std::uniform_real_distribution<units::Coordinate> distY(region.top(), region.bottom());
	const units::Coordinate2D cen(distX(rando), distY(rando));
	
	if (minVerts < 3 || maxVerts < 3) std::cerr << "Error: Cannot generate a polygon with fewer than 3 vertices. Defaulting to 3 minimum.\n";
	const std::size_t min = minVerts < 3 ? 3 : minVerts;
	std::uniform_int_distribution<std::size_t> distVerts(min, maxVerts < min ? min : maxVerts);
	const std::size_t numVerts(distVerts(rando));

	// Generate random numbers between 0 and tau (2pi) to make points around a circle.
	std::uniform_real_distribution<units::Coordinate> distPI(0.0f, constants::TAU);
	std::vector<units::Coordinate> piVec;
	piVec.reserve(numVerts);
	for (std::size_t i = 0; i < numVerts; ++i) {
		piVec.push_back(distPI(rando));
	}

	// Sort descending (so we have counterclockwise winding).
	std::sort(piVec.begin(), piVec.end(), [](const units::Coordinate& lhs, const units::Coordinate& rhs) {
		return lhs > rhs;
	});

	// Get radius for polygon.
	std::uniform_real_distribution<units::Coordinate> distRad(minRad, maxRad);
	units::Coordinate radius(distRad(rando));

	std::vector<units::Coordinate2D> vertices;
	vertices.reserve(numVerts);
	for (std::size_t i = 0; i < numVerts; ++i) {
		vertices.push_back(cen + units::Coordinate2D(radius * std::cos(piVec[i]), radius * std::sin(piVec[i])));
	}

	return Polygon(vertices);
}

const units::Coordinate2D& Polygon::getEdgeNorm(std::size_t index) const {
	if (!edge_normals_[index].isZero())
		return edge_normals_[index];
	const units::Coordinate2D first = vertices_[index];
	const units::Coordinate2D second = vertices_[index + 1 >= vertices_.size() ? 0 : index + 1];
	edge_normals_[index] = units::Coordinate2D(first.y - second.y, second.x - first.x).normalize();
	return edge_normals_[index];
}

void Polygon::computeNormals() const {
	for (std::size_t i = 0; i < edge_normals_.size(); ++i)
		getEdgeNorm(i);
}

Projection Polygon::getProjection(const units::Coordinate2D& axis) const {
	units::Coordinate min(vertices_[0].dot(axis));
	units::Coordinate max(min);
	units::Coordinate proj;
	for (std::size_t i = 1; i < vertices_.size(); ++i) {
		proj = vertices_[i].dot(axis);
		if (proj < min)
			min = proj;
		else if (proj > max)
			max = proj;
	}
	return Projection(min, max);
}

void Polygon::expand(const units::Coordinate expandAmount) {
	if (expandAmount < 0) {
		std::cerr << "Error: Cannot expand a polygon by negative amounts.\n";
		return;
	}
	std::vector<units::Coordinate2D> newVertices;
	newVertices.reserve(vertices_.size());
	for (std::size_t i = 0; i < vertices_.size(); ++i) {
		units::Coordinate2D vertexNorm = (getEdgeNorm(i == 0 ? vertices_.size()-1 : i-1) + getEdgeNorm(i)).normalize();
		newVertices.push_back(vertices_[i] + vertexNorm*expandAmount);
	}
	for (std::size_t i = 0; i < vertices_.size(); ++i)
		vertices_[i] = newVertices[i];
	_find_bounds();
}
Polygon Polygon::expand(const Polygon& p, const units::Coordinate expandAmount) {
	Polygon s(p);
	s.expand(expandAmount);
	return s;
}

namespace {
	// Compute the normal of an edge of a polygon with counterclockwise winding, without normalizing it to a unit vector.
	inline units::Coordinate2D _get_non_normalized_normal(const units::Coordinate2D& first, const units::Coordinate2D& second) {
		return units::Coordinate2D(first.y - second.y, second.x - first.x);
	}
	enum AngleResult {
		ACUTE,
		PERPENDICULAR,
		OBTUSE
	};
	// Check what kind of angle the minimum angle between two vectors is.
	inline AngleResult _check_min_angle(const units::Coordinate2D& vec1, const units::Coordinate2D& vec2) {
		const units::Coordinate dot = vec1.dot(vec2);
		if (std::abs(dot) <= Polygon::EPSILON_DEGREE_TOLERANCE)
			return PERPENDICULAR;
		if (dot > 0)
			return ACUTE;
		return OBTUSE;
	}
}

// Assumes that dir is not a zero vector.
bool Polygon::findExtendRange(const units::Coordinate2D& dir, std::size_t& out_first, std::size_t& out_last,
							  bool& out_should_dupe_first, bool& out_should_dupe_last) const {
	const std::size_t numVerts = vertices_.size();
	if (numVerts < 3) {
		std::cerr << "Error: Cannot extend polygon (polygon invalid; must have at least three vertices).\n";
		return false;
	}
	// We are interested in extending from vertices that have at least one edge normal with a minimum angle acute to the direction vector.
	// With a convex polygon, there will form a single contiguous range of such vertices.
	// The first and last vertex in that range may need to be duplicated, and then the vertices within the range
	// are projected along the direction vector by delta to form the new polygon.
	// The first and last vertices are defined by the vertices that have only one acute edge normal.

	// Whether the minimum angle of the normal of the edge made from the last and first vertices is acute with the direction vector.
	const AngleResult firstEdge = _check_min_angle(_get_non_normalized_normal(vertices_[numVerts-1], vertices_[0]), dir);
	const bool isFirstEdgeAcute = firstEdge == ACUTE;

	AngleResult prevEdge = firstEdge;
	AngleResult currEdge;
	bool found = false;
	std::size_t vertexInRegion;
	for (std::size_t i = 0; i < numVerts - 1; ++i) {
		currEdge = _check_min_angle(_get_non_normalized_normal(vertices_[i], vertices_[i+1]), dir);
		if (isFirstEdgeAcute != (currEdge == ACUTE)) {
			// Either crossed from inside to outside the region, or vice versa.
			// (One side of the vertex has an edge normal that is acute, the other side obtuse.)
			found = true;
			vertexInRegion = i;
			break;
		}
		prevEdge = currEdge;
	}
	if (!found) {
		// A valid polygon has two points that define where the region starts and ends.
		// If we didn't find one in the loop, the polygon is invalid.
		std::cerr << "Error: Polygon can not be extended (invalid polygon).\n";
		return false;
	}
	// We found either the first or last vertex for the region.
	if (isFirstEdgeAcute) {
		// It is the last vertex in the region.
		out_last = vertexInRegion;
		out_should_dupe_last = currEdge != PERPENDICULAR; // If perpendicular, don't need to duplicate the vertex when extending.
		// Loop backwards from the end to find the first vertex.
		for (std::size_t i = numVerts - 1; i > 0; --i) {
			currEdge = _check_min_angle(_get_non_normalized_normal(vertices_[i-1], vertices_[i]), dir);
			if (currEdge != ACUTE) {
				out_first = i;
				out_should_dupe_first = currEdge != PERPENDICULAR;
				return true;
			}
		}
		std::cerr << "Error: Polygon can not be extended (invalid polygon).\n";
		return false;
	}
	// Otherwise it is the first vertex in the region.
	out_first = vertexInRegion;
	out_should_dupe_first = prevEdge != PERPENDICULAR; // If perpendicular, don't need to duplicate the vertex when extending.
	// Loop forwards from the first vertex to find where it ends.
	for (std::size_t i = vertexInRegion + 1; i < numVerts - 1; ++i) {
		currEdge = _check_min_angle(_get_non_normalized_normal(vertices_[i], vertices_[i+1]), dir);
		if (currEdge != ACUTE) {
			out_last = i;
			out_should_dupe_last = currEdge != PERPENDICULAR;
			return true;
		}
	}
	// The edge normal between the last and first vertex is the only non-acute edge normal.
	out_last = numVerts - 1;
	out_should_dupe_last = firstEdge != PERPENDICULAR;
	return true;
}

Polygon Polygon::extend(const units::Coordinate2D& dir, const units::Coordinate dist) const {
	if (dir.isZero() || dist == 0.0f) { // No delta. Just return the current polygon.
		return Polygon(*this);
	}
	std::size_t first, last;
	bool shouldDuplicateFirst, shouldDuplicateLast;
	if ( !findExtendRange(dir, first, last, shouldDuplicateFirst, shouldDuplicateLast) ) {
		return Polygon(); // The polygon is invalid and cannot be extended.
	}
	return extend(dir, dist, first, last, shouldDuplicateFirst, shouldDuplicateLast);
}

Polygon Polygon::clipExtend(const units::Coordinate2D& dir, const units::Coordinate dist) const {
	if (dir.isZero() || dist == 0.0f) { // No delta. Just return the current polygon.
		return Polygon(*this);
	}
	std::size_t first, last;
	bool shouldDuplicateFirst, shouldDuplicateLast; // Note that we always duplicate here.
	if ( !findExtendRange(dir, first, last, shouldDuplicateFirst, shouldDuplicateLast) ) {
		return Polygon(); // The polygon is invalid and cannot be extended.
	}
	return clipExtend(dir, dist, first, last);
}

Polygon Polygon::extend(const units::Coordinate2D& dir, const units::Coordinate dist,
						const std::size_t rangeFirst, const std::size_t rangeLast, const bool shouldDupeFirst, const bool shouldDupeLast) const {
	std::vector<units::Coordinate2D> newVertices, newEdgeNorms;
	const std::size_t numVerts = vertices_.size() + (shouldDupeFirst ? 1 : 0) + (shouldDupeLast ? 1 : 0);
	newVertices.reserve(numVerts);
	newEdgeNorms.reserve(numVerts);
	const units::Coordinate2D translation(dir*dist);
	for (std::size_t i = 0; i < vertices_.size(); ++i) {
		// Extend vertices in the region first-to-last inclusive. Duplicate first/last vertices if required.
		if (i == rangeFirst && shouldDupeFirst) {
			newVertices.push_back(vertices_[i]);
			newVertices.push_back(vertices_[i] + translation);
			newEdgeNorms.push_back(dir.perpCCW());
			newEdgeNorms.push_back(edge_normals_[i]);
		} else if (i == rangeLast && shouldDupeLast) {
			newVertices.push_back(vertices_[i] + translation);
			newVertices.push_back(vertices_[i]);
			newEdgeNorms.push_back(dir.perpCW());
			newEdgeNorms.push_back(edge_normals_[i]);
		} else {
			newVertices.push_back( rangeFirst > rangeLast ? // Determine which range to use.
				( (i <= rangeLast || i >= rangeFirst) ? vertices_[i] + translation : vertices_[i] ) : // Range overlaps end/start of the array.
				( (i <= rangeLast && i >= rangeFirst) ? vertices_[i] + translation : vertices_[i] )); // Range is somewhere in the middle of the array.
			newEdgeNorms.push_back(edge_normals_[i]);
		}
	}
	return Polygon(newVertices, newEdgeNorms);
}
Polygon Polygon::clipExtend(const units::Coordinate2D& dir, const units::Coordinate dist, const std::size_t rangeFirst, const std::size_t rangeLast) const {
	std::vector<units::Coordinate2D> newVertices, newEdgeNorms;
	// Since we always duplicate when clipping, we will have last-to-first inclusive + 2x duplicates.
	const std::size_t numVerts = std::abs(static_cast<int>(rangeLast) - static_cast<int>(rangeFirst)) + 3;
	newVertices.reserve(numVerts);
	newEdgeNorms.reserve(numVerts);
	newVertices.push_back(vertices_[rangeFirst]); // First vertex gets duplicated.
	newEdgeNorms.push_back(dir.perpCCW());
	const units::Coordinate2D translation(dir*dist);
	if ( rangeFirst < rangeLast ) {
		for (std::size_t i = rangeFirst; i < rangeLast; ++i) {
			newVertices.push_back(vertices_[i] + translation);
			newEdgeNorms.push_back(edge_normals_[i]);
		}
	} else { // Range between first and last overlaps start/end of the array.
		for (std::size_t i = rangeFirst; i < vertices_.size(); ++i) {
			newVertices.push_back(vertices_[i] + translation);
			newEdgeNorms.push_back(edge_normals_[i]);
		}
		for (std::size_t i = 0; i < rangeLast; ++i) {
			newVertices.push_back(vertices_[i] + translation);
			newEdgeNorms.push_back(edge_normals_[i]);
		}
	}
	newVertices.push_back(vertices_[rangeLast] + translation);
	newVertices.push_back(vertices_[rangeLast]); // Last vertex gets duplicated.
	newEdgeNorms.push_back(dir.perpCW());
	newEdgeNorms.push_back(units::Coordinate2D(0,0)); // Space for last edge normal, but not computed.
	return Polygon(newVertices, newEdgeNorms);
}

void Polygon::translate(const units::Coordinate x, const units::Coordinate y) {
	this->translate(units::Coordinate2D(x, y));
}

void Polygon::translate(const units::Coordinate2D& delta) {
	for (std::size_t i = 0; i < vertices_.size(); ++i) {
		vertices_[i] += delta;
	}
	x_min_ += delta.x;
	x_max_ += delta.x;
	y_min_ += delta.y;
	y_max_ += delta.y;
}

Polygon Polygon::translate(const Polygon& p, const units::Coordinate2D& delta) {
	Polygon t(p);
	t.translate(delta);
	return t;
}

Polygon Polygon::toPoly() const {
	return Polygon(*this);
}