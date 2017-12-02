#ifndef _LINE_SEGMENT_H
#define _LINE_SEGMENT_H

#include "Units.h"

class LineSegment {

public:
	units::Coordinate2D start, end;

	LineSegment() : start(), end() {}
	LineSegment(units::Coordinate2D start, units::Coordinate2D end) : start(start), end(end) {}
	LineSegment(units::Coordinate x1, units::Coordinate y1, units::Coordinate x2, units::Coordinate y2)	: start(x1,y1), end(x2,y2) {}

	inline bool isPoint() const { return start.x == end.x && start.y == end.y; }
	inline units::Coordinate magnitude2() const { return (end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y); }
	inline units::Coordinate magnitude() const { return std::sqrt( magnitude2() ); }

	inline units::Coordinate min_x() const { return start.x < end.x ? start.x : end.x; }
	inline units::Coordinate max_x() const { return start.x > end.x ? start.x : end.x; }
	inline units::Coordinate min_y() const { return start.y < end.y ? start.y : end.y; }
	inline units::Coordinate max_y() const { return start.y > end.y ? start.y : end.y; }

	// Get counter-clockwise (non-normalized) normal for the line segment. (Rotate 90 degrees CCW.)
	inline units::Coordinate2D getNormalCCW() const { return units::Coordinate2D(start.y - end.y, end.x - start.x); }
	// Get normalized counter-clockwise normal for the line segment. (Rotate 90 degrees CCW and normalize.)
	inline units::Coordinate2D getNormalizedNormalCCW() const { return getNormalCCW().normalize(); }
};

#endif // _LINE_SEGMENT_H