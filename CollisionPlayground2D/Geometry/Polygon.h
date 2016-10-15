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

	// Compute the normals for each vertex of the polygon.
	// To be used, for example, if the polygon is often used with the extend function.
	void computeVertexNormals();

	// Rather than recompute the bounds every call, the bounds
	// are computed when the polygon is constructed or changed,
	// and then cached to be returned here.
	virtual units::Coordinate left()   const { return x_min_; }
	virtual units::Coordinate right()  const { return x_max_; }
	virtual units::Coordinate top()    const { return y_min_; }
	virtual units::Coordinate bottom() const { return y_max_; }

	// Extend a polygon by projecting it along a delta vector.
	virtual Polygon extend(units::Coordinate2D delta) const;

	virtual Polygon toPoly() const;


	void draw(Graphics& graphics, bool isColliding) const;
	void drawNormals(Graphics& graphics) const;
};

#endif //_POLYGON_H