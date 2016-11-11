#ifndef _LINE_SEGMENT_H
#define _LINE_SEGMENT_H

#include "../Units.h"

class LineSegment {

public:
	units::Coordinate2D start, end;

	LineSegment() : start(), end() {}
	LineSegment(units::Coordinate2D start, units::Coordinate2D end) : start(start), end(end) {}
	LineSegment(units::Coordinate x1, units::Coordinate y1, units::Coordinate x2, units::Coordinate y2)	: start(x1,y1), end(x2,y2) {}

	inline bool isPoint() const;

	units::Coordinate magnitude2() const;
	units::Coordinate magnitude() const;

	inline units::Coordinate min_x() const { return start.x < end.x ? start.x : end.x; }
	inline units::Coordinate max_x() const { return start.x > end.x ? start.x : end.x; }
	inline units::Coordinate min_y() const { return start.y < end.y ? start.y : end.y; }
	inline units::Coordinate max_y() const { return start.y > end.y ? start.y : end.y; }

	bool intersects(const units::Coordinate2D point) const;
	// This version will put the intersection point in out_intersection.
	bool intersects(const LineSegment& o, units::Coordinate2D& out_intersection) const;
	// This version should be faster if the intersection point isn't needed.
	bool intersects(const LineSegment& o) const;
};

#endif // _LINE_SEGMENT_H