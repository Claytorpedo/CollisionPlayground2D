#ifndef RECTANGLE_H_
#define RECTANGLE_H_

#include <SDL.h>
#include <iostream>
#include <string>

#include "Shape.h"
#include "LineSegment.h"

using namespace units;

class Rectangle : public Shape {
public:
	Coordinate x, y, w, h;
	Uint8 r, g, b, a;

	Rectangle() : x(0.0f), y(0.0f), w(0.0f), h(0.0f), r(0), g(0), b(255), a(255){}
	Rectangle(Coordinate x, Coordinate y, Coordinate w, Coordinate h) : x(x), y(y), w(w), h(h), r(0), g(0), b(255), a(255){}
	Rectangle(Coordinate2D topLeft, Coordinate w, Coordinate h) : x(topLeft.x), y(topLeft.y), w(w), h(h), r(0), g(0), b(255), a(255){}
	Rectangle(const Rectangle& rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h), r(0), g(0), b(255), a(255){}

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

	SDL_Rect convertToSDLRect() const;
	bool isInside(const Rectangle& other) const;
	bool collides(const Rectangle& other) const;
	bool collides(const units::Coordinate2D point) const;
	bool collides(const LineSegment& line) const;

	virtual void draw(Graphics& graphics, bool isColliding) const;
	void draw(Graphics& graphics, bool isColliding, Uint8 thickness) const;
	void setColour(Uint8 r, Uint8 g, Uint8 b, Uint8 a);

	virtual Polygon toPoly() const;
};

#endif // RECTANGLE_H_