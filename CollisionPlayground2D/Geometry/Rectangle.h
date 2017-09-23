#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include "Shape.h"
#include "Polygon.h"

using namespace units;

class Rectangle : public Shape {
public:
	Coordinate x, y, w, h;

	Rectangle() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}
	Rectangle(Coordinate x, Coordinate y, Coordinate w, Coordinate h) : x(x), y(y), w(w), h(h) {}
	Rectangle(Coordinate2D topLeft, Coordinate w, Coordinate h) : x(topLeft.x), y(topLeft.y), w(w), h(h) {}
	Rectangle(const Rectangle& rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h) {}

	// Translate by a vector.
	inline Rectangle& operator+=(const units::Coordinate2D& c) { x += c.x; y += c.y; return *this; }
	// Translate by a vector.
	inline Rectangle operator+(const units::Coordinate2D& c) { return Rectangle(x + c.x, y + c.y, w, h); }

	inline Coordinate2D position() const { return Coordinate2D(x, y); }
	inline Coordinate2D center()   const { return Coordinate2D(x + w*0.5f, y + h*0.5f); }
	inline Coordinate center_x() const { return x + w*0.5f; }
	inline Coordinate center_y() const { return y + h*0.5f; }
	inline Coordinate width()    const { return w; }
	inline Coordinate height()   const { return h; }
	inline virtual Coordinate left()   const { return x; }
	inline virtual Coordinate right()  const { return x + w; }
	inline virtual Coordinate top()    const { return y; }
	inline virtual Coordinate bottom() const { return y + h; }

	inline Coordinate2D topLeft()     const { return Coordinate2D(left(),  top()); }
	inline Coordinate2D topRight()    const { return Coordinate2D(right(), top()); }
	inline Coordinate2D bottomLeft()  const { return Coordinate2D(left(),  bottom()); }
	inline Coordinate2D bottomRight() const { return Coordinate2D(right(), bottom()); }

	inline bool isInside(const Rectangle& o) const {
		return right() <= o.right() + constants::EPSILON && bottom() <= o.bottom() + constants::EPSILON && 
		       left() >= o.left() - constants::EPSILON   && top() >= o.top() - constants::EPSILON;
	}
	
	virtual Polygon toPoly() const {
		std::vector<units::Coordinate2D> vertices;
		vertices.reserve(4);
		vertices.push_back(topLeft());
		vertices.push_back(bottomLeft());
		vertices.push_back(bottomRight());
		vertices.push_back(topRight());
		return Polygon(vertices);
	}
};

#endif // RECTANGLE_H_