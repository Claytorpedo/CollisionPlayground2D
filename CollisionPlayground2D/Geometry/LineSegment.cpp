#include "LineSegment.h"

#include "../Units.h"
#include "../Constants.h"

namespace {
	inline char _compute_direction(const units::Coordinate2D& a, const units::Coordinate2D& b, const units::Coordinate2D& c ) {
		units::Coordinate p = (c.x - a.x) * (b.y - a.y);
		units::Coordinate q = (b.x - a.x) * (c.y - a.y);
		return p < q ? -1 : p > q ? 1 : 0;
	}
	inline bool _is_on_segment(const units::Coordinate2D& a, const units::Coordinate2D& b, const units::Coordinate2D& c) {
		return	(a.x <= c.x || b.x <= c.x) && (c.x <= a.x || c.x <= b.x) &&
			    (a.y <= c.y || b.y <= c.y) && (c.y <= a.y || c.y <= b.y);
	}
}

bool LineSegment::isPoint() const { return start.x == end.x && start.y == end.y; }

units::Coordinate LineSegment::magnitudeSquared() const {
	return (end.x - start.x) * (end.x - start.x) + (end.y - start.y) * (end.y - start.y);
}

units::Coordinate LineSegment::magnitude() const {
	return std::sqrt( magnitudeSquared() );
}

bool LineSegment::intersects(const units::Coordinate2D point) const {
	// Check bounding box.
	if (point.x >= min_x() && point.x <= max_x() && point.y <= min_y() && point.y >= max_y()) {
		const units::Coordinate denom = end.x - start.x;
		if (std::abs(denom) < constants::EPSILON) {
			// Vertical line. See if they share an x value.
			return std::abs(point.x - start.x) < constants::EPSILON;
		}
		const units::Coordinate numer = end.y - start.y;
		if (std::abs(numer) < constants::EPSILON) {
			// Horizontal line. See if they share a y value.
			return std::abs(point.y - start.y) < constants::EPSILON;
		}
		const units::Coordinate slope = numer/denom;
		const units::Coordinate intercept = start.y - slope*start.x; // b = y - mx
		const units::Coordinate q = slope*point.x + intercept; // y = mx + b
		// Check with line equation.
		if ( std::abs(q - point.y) < constants::EPSILON) {
			return true;
		}
	}
	return false;
}
// This version will put the intersection point in out_intersection if it exists.
bool LineSegment::intersects(const LineSegment& o, units::Coordinate2D& out_intersection) const {
	// Check if either or both segments are a point.
	if (isPoint()) {
		if (start == o.start) {
			// This catches the case where o is also a point.
			out_intersection = start;
			return true;
		}
		if (o.intersects(start)) {
			out_intersection = start;
			return true;
		}
		return false;
	}
	if (o.isPoint()) {
		if (intersects(o.start)) {
			out_intersection = o.start;
			return true;
		}
		return false;
	}
	const units::Coordinate2D r = end - start;
	const units::Coordinate2D s = o.end - o.start;
	const units::Coordinate2D qp = o.start - start;
	const units::Coordinate rxs = r.cross(s);
	const units::Coordinate qpxr = qp.cross(r);
	if (rxs == 0 && qpxr == 0) {
		// Lines are colinear. Test for overlap.
		// Don't have to worry about r being zero, since we tested for points already.
		const units::Coordinate recip = 1/r.dot(r); // Save a division.
		const units::Coordinate t0 = qp.dot(r) * recip;
		const units::Coordinate s_dot_r = s.dot(r);
		const units::Coordinate t1 = t0 + s_dot_r * recip;
		// See if the lines overlap.
		if (s_dot_r < 0) {
			if (0 <= t0 && t1 <= 1) {
				// Get closest intersect (to start point) for the segment this is called on.
				out_intersection = start + t0*r;
				return true;
			}
		} else {
			if (0 <= t1 && t0 <= 1) {
				// Get closest intersect (to start point) for the segment this is called on.
				out_intersection = start + t1*r;
				return true;
			}
		}			
		return false;
	}
	if (rxs == 0 && qpxr != 0) {
		// Parallel and non-intersecting.
		return false;
	}
	// Guaranteed that rxs != 0 here.
	const units::Coordinate recip = 1/rxs; // Save a division.
	const units::Coordinate t = qp.cross(s) * recip;
	const units::Coordinate u = qpxr * recip;
	if (0 <= t && t <= 1 && 0 <= u && u <= 1) {
		// They intersect.
		out_intersection = start + t*r;
		return true;
	}
	// No intersection.
	return false;
}
// This version should be faster if the intersection point isn't needed.
bool LineSegment::intersects(const LineSegment& o) const {
	char d1 = _compute_direction(o.start, o.end, start);
	char d2 = _compute_direction(o.start, o.end, end);
	char d3 = _compute_direction(start, end, o.start);
	char d4 = _compute_direction(start, end, o.end);
	return (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) ||
		(d1 == 0 && _is_on_segment(o.start, o.end, start)) ||
		(d2 == 0 && _is_on_segment(o.start, o.end, end)) ||
		(d3 == 0 && _is_on_segment(start, end, o.start)) ||
		(d4 == 0 && _is_on_segment(start, end, o.end));
}