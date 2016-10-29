#ifndef _POLYGON_H
#define _POLYGON_H

#include "Shape.h"

#include <vector>

// Convex polygon with counterclockwise winding.

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

	// Rather than recompute the bounds every call, the bounds
	// are computed when the polygon is constructed or changed,
	// and then cached to be returned here.
	virtual units::Coordinate left()   const { return x_min_; }
	virtual units::Coordinate right()  const { return x_max_; }
	virtual units::Coordinate top()    const { return y_min_; }
	virtual units::Coordinate bottom() const { return y_max_; }

	// Extend a polygon by projecting it along a delta vector.
	virtual Polygon extend(units::Coordinate2D delta) const;

	virtual Polygon toPoly() const; // Returns a copy of itself.

	// Find the first and last vertices in the range to extend from in the direction of the given delta vector.
	// Also finds whether the first and last vertices need to be duplicated when extending.
	// Note that first and last follow the winding of the polygon, and thus can overlap between the end and
	// start of the Polygon's vertex list (resulting in first > last).
	// Returns false if the range could not be found, indicating an invalid polygon.
	bool findExtendRange(units::Coordinate2D delta, std::size_t& out_first, std::size_t& out_last,
		bool& out_should_dupe_first, bool& out_should_dupe_last) const;
	// Extend a polygon by projecting it along a delta vector, clipping the result to only include
	// the portion of the polygon that was extended.
	Polygon clipExtend(units::Coordinate2D delta) const;
	// If we've already found the range, we can clip extend with the found values.
	Polygon clipExtend(units::Coordinate2D delta, std::size_t rangeFirst, std::size_t rangeLast) const;
	// If we've already found the range, we can extend with the found values.
	Polygon extend(units::Coordinate2D delta, std::size_t rangeFirst, std::size_t rangeLast, bool shouldDupeFirst, bool shouldDupeLast) const;

	// For accessing the values of the vertices of the polygon. Note no safety checks.
	inline units::Coordinate2D operator[](std::size_t index) const { return vertices_[index]; }
	// Get the number of vertices in the polygon.
	inline std::size_t size() const { return vertices_.size(); }
	// Check if the polygon has any vertices.
	inline bool isEmpty() const { return vertices_.empty(); }


	void draw(Graphics& graphics, bool isColliding) const;
	void drawEdgeNormals(Graphics& graphics) const;
};

#endif //_POLYGON_H