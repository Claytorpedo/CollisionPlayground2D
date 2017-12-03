#ifndef _GEOM_RECTANGLE_H
#define _GEOM_RECTANGLE_H

#include "Units.h"
#include "Shape.h"
#include "Polygon.h"
#include "Projection.h"

namespace geom {
	class Rectangle : public Shape {
	public:
		gFloat x, y, w, h;

		Rectangle() : x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}
		Rectangle(gFloat x, gFloat y, gFloat w, gFloat h) : x(x), y(y), w(w), h(h) {}
		Rectangle(Coord2 topLeft, gFloat w, gFloat h) : x(topLeft.x), y(topLeft.y), w(w), h(h) {}
		Rectangle(const Rectangle& rect) : x(rect.x), y(rect.y), w(rect.w), h(rect.h) {}

		// Translate by a vector.
		inline Rectangle& operator+=(const Coord2& c) { x += c.x; y += c.y; return *this; }
		// Translate by a vector.
		inline Rectangle operator+(const Coord2& c) { return Rectangle(x + c.x, y + c.y, w, h); }

		inline Coord2 position() const { return Coord2(x, y); }
		inline Coord2 center()   const { return Coord2(x + w*0.5f, y + h*0.5f); }
		inline gFloat center_x() const { return x + w*0.5f; }
		inline gFloat center_y() const { return y + h*0.5f; }
		inline gFloat width()    const { return w; }
		inline gFloat height()   const { return h; }
		inline virtual gFloat left()   const { return x; }
		inline virtual gFloat right()  const { return x + w; }
		inline virtual gFloat top()    const { return y; }
		inline virtual gFloat bottom() const { return y + h; }

		inline Coord2 topLeft()     const { return Coord2(left(), top()); }
		inline Coord2 topRight()    const { return Coord2(right(), top()); }
		inline Coord2 bottomLeft()  const { return Coord2(left(), bottom()); }
		inline Coord2 bottomRight() const { return Coord2(right(), bottom()); }

		inline bool isInside(const Rectangle& o) const; // See if this rectangle is contained by another one.

		virtual Projection getProjection(const Coord2& axis) const;
		virtual Coord2 getClosestTo(const Coord2 point) const; // Gets closest corner of the rectangle.
		virtual Polygon toPoly() const;
	};
}
#endif // _GEOM_RECTANGLE_H_