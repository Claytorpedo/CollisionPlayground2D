#include "Rectangle.h"

#include <SDL.h>

#include "LineSegment.h"
#include "Polygon.h"

SDL_Rect Rectangle::convertToSDLRect() const {
	SDL_Rect rect = { static_cast<int>(x), static_cast<int>(y), static_cast<int>(w), static_cast<int>(h) };
	return rect;
}

bool Rectangle::isInside(const Rectangle& o) const {
	return	right() <= o.right() + constants::EPSILON && bottom() <= o.bottom() + constants::EPSILON && 
		left() >= o.left() - constants::EPSILON && top() >= o.top() - constants::EPSILON;
}
bool Rectangle::collides(const Rectangle& o) const { 
	return ( left() < o.right() && right() > o.left() && top() < o.bottom() && bottom() > o.top() );
}
bool Rectangle::collides(const units::Coordinate2D p) const {
	return p.x >= x && p.y >= y && p.x <= right() && p.y <= bottom();
}
bool Rectangle::collides(const LineSegment& l) const {
	// Check if either endpoints are inside the rectangle.
	if ((l.min_x() > left() && l.min_x() < right() && l.min_y() > bottom() && l.min_y() < top()) ||
		(l.max_x() > left() && l.max_x() < right() && l.max_y() > bottom() && l.max_y() < top()))
		return true;
	// Test l against 4 line segments that make up the rectangle
	LineSegment leftSide = LineSegment(left(), bottom(), left(), top()); // Left side.
	if (leftSide.intersects(l))
		return true;
	LineSegment topSide = LineSegment(left(), top(), right(), top()); // Top side.
	if (topSide.intersects(l))
		return true;
	LineSegment rightSide = LineSegment(right(), bottom(), right(), top()); // Right side.
	if (rightSide.intersects(l))
		return true;
	LineSegment bottomSide = LineSegment(left(), bottom(), right(), bottom()); // Bottom side.
	if (bottomSide.intersects(l))
		return true;
	return false;
}

void Rectangle::draw(Graphics& graphics, bool isColliding) const {
	graphics.renderRect(convertToSDLRect(), isColliding ? 255 : 0, isColliding ? 0 : 255, isColliding ? 0 : 255, 255);
}

Polygon Rectangle::toPoly() const {
	std::vector<units::Coordinate2D> vertices(4);
	vertices[0] = units::Coordinate2D(x, y); // Top left.
	vertices[1] = units::Coordinate2D(x, bottom()); // Bottom left.
	vertices[2] = units::Coordinate2D(right(), bottom()); // Bottom right.
	vertices[3] = units::Coordinate2D(right(), y); // Top right.
	return Polygon(vertices);
}