#ifndef _RECTANGLE_H
#define _RECTANGLE_H

#include "Units.h"
#include "Shape.h"
#include "Polygon.h"
#include "Projection.h"

class Rectangle : public Shape {
public:
	units::Coordinate x, y, w, h;

	Rectangle() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}
	Rectangle(units::Coordinate x, units::Coordinate y, units::Coordinate w, units::Coordinate h) : x(x), y(y), w(w), h(h) {}
	Rectangle(units::Coordinate2D topLeft, units::Coordinate w, units::Coordinate h) : x(topLeft.x), y(topLeft.y), w(w), h(h) {}
	Rectangle(const Rectangle& rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h) {}

	// Translate by a vector.
	inline Rectangle& operator+=(const units::Coordinate2D& c) { x += c.x; y += c.y; return *this; }
	// Translate by a vector.
	inline Rectangle operator+(const units::Coordinate2D& c) { return Rectangle(x + c.x, y + c.y, w, h); }

	inline units::Coordinate2D position() const { return units::Coordinate2D(x, y); }
	inline units::Coordinate2D center()   const { return units::Coordinate2D(x + w*0.5f, y + h*0.5f); }
	inline units::Coordinate center_x() const { return x + w*0.5f; }
	inline units::Coordinate center_y() const { return y + h*0.5f; }
	inline units::Coordinate width()    const { return w; }
	inline units::Coordinate height()   const { return h; }
	inline virtual units::Coordinate left()   const { return x; }
	inline virtual units::Coordinate right()  const { return x + w; }
	inline virtual units::Coordinate top()    const { return y; }
	inline virtual units::Coordinate bottom() const { return y + h; }

	inline units::Coordinate2D topLeft()     const { return units::Coordinate2D(left(),  top()); }
	inline units::Coordinate2D topRight()    const { return units::Coordinate2D(right(), top()); }
	inline units::Coordinate2D bottomLeft()  const { return units::Coordinate2D(left(),  bottom()); }
	inline units::Coordinate2D bottomRight() const { return units::Coordinate2D(right(), bottom()); }

	inline bool isInside(const Rectangle& o) const; // See if this rectangle is contained by another one.
	
	virtual Projection getProjection(const units::Coordinate2D& axis) const;
	virtual units::Coordinate2D getClosestTo(const units::Coordinate2D point) const; // Gets closest corner of the rectangle.
	virtual Polygon toPoly() const;
};

#endif // _RECTANGLE_H_