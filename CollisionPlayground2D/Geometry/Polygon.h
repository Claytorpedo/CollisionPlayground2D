#ifndef _POLYGON_H
#define _POLYGON_H

#include "Shape.h"

#include <vector>
#include <random>

class Rectangle;

// Convex polygon with counterclockwise winding.

namespace polygon {
	// Help avoid accumulative floating point errors with an epsilon.
	// Used when determining the type of angle between two vectors with a dot product.
	// Because we don't normalize the edge vectors, the choice of epsilon is even more important.
	// Choosing an epsilon that fits the scale of the polygons being used is important, or
	// there will be errors when dealing with near-perpendicular lines.
	const units::Coordinate EPSILON_DEGREE_TOLERANCE = 0.00001f;
}

class Polygon : public Shape {
private:
	std::vector<units::Coordinate2D> vertices_;
	units::Coordinate x_min_, x_max_, y_min_, y_max_;
	
	// Find the bounding box for the polygon and cache the values.
	void findBounds();
public:
	Polygon();
	// Vertices should be in counterclockwise order.
	// The final vertex connects with the first vertex.
	Polygon(std::vector<units::Coordinate2D> vertices);
	Polygon(const Polygon& poly);
	~Polygon();

	// Randomly generate a polygon.
	// region is a bounding box defining the region to place the polygon's center in (part of the polygon can be outside this region).
	// minRad and maxRad control how large the generated polygon will be.
	// minVerts and maxVerts control how many vertices the generated polygon can have.
	static Polygon generate(std::mt19937& rando, const Rectangle& region,
		const units::Coordinate minRad=0.1f, const units::Coordinate maxRad=100.0f,  const std::size_t minVerts=3, const std::size_t maxVerts=20);

	// Rather than recompute the bounds every call, the bounds
	// are computed when the polygon is constructed or changed,
	// and then cached to be returned here.
	virtual units::Coordinate left()   const { return x_min_; }
	virtual units::Coordinate right()  const { return x_max_; }
	virtual units::Coordinate top()    const { return y_min_; }
	virtual units::Coordinate bottom() const { return y_max_; }

	// Extend a polygon by projecting it along a direction by dist.
	virtual Polygon extend(const units::Coordinate2D& dir, const units::Coordinate dist) const;

	virtual Polygon toPoly() const; // Returns a copy of itself.

	// Find the first and last vertices in the range to extend from in the direction of the given direction vector.
	// Also finds whether the first and last vertices need to be duplicated when extending.
	// Note that first and last follow the winding of the polygon, and thus can overlap between the end and
	// start of the Polygon's vertex list (resulting in first > last).
	// Returns false if the range could not be found, indicating an invalid polygon.
	bool findExtendRange(const units::Coordinate2D& dir, std::size_t& out_first, std::size_t& out_last,
		bool& out_should_dupe_first, bool& out_should_dupe_last) const;
	// Extend a polygon by projecting it along a direction by delta (dir*dist), clipping the result to only include
	// the portion of the polygon that was extended.
	Polygon clipExtend(const units::Coordinate2D& dir, const units::Coordinate dist) const;
	// If we've already found the range, we can clip extend with the found values.
	Polygon clipExtend(const units::Coordinate2D& dir, const units::Coordinate dist,
		const std::size_t rangeFirst, const std::size_t rangeLast) const;
	// If we've already found the range, we can extend with the found values.
	Polygon extend(const units::Coordinate2D& dir, const units::Coordinate dist,
		const std::size_t rangeFirst, const std::size_t rangeLast, const bool shouldDupeFirst, const bool shouldDupeLast) const;

	// Move the polygon by given x and y.
	void translate(const units::Coordinate x, const units::Coordinate y);
	// Move the polygon by delta.
	void translate(const units::Coordinate2D& delta);

	// For accessing the values of the vertices of the polygon. Note no safety checks.
	inline units::Coordinate2D operator[](std::size_t index) const { return vertices_[index]; }
	// Get the number of vertices in the polygon.
	inline std::size_t size() const { return vertices_.size(); }
	// Check if the polygon has any vertices.
	inline bool isEmpty() const { return vertices_.empty(); }
};

#endif //_POLYGON_H