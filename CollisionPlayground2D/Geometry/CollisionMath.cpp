#include "../Units.h"
#include "../Util.h"

#include "LineSegment.h"
#include "Ray.h"

namespace collision_math {

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

	// ------------------------------- Point intersections --------------------------------------------------

	bool intersects(const LineSegment& l, const units::Coordinate2D p) {
		// Check bounding box.
		if ((p.x + constants::EPSILON) < l.min_x() || (p.x - constants::EPSILON) > l.max_x() ||
			(p.y + constants::EPSILON) < l.min_y() || (p.y - constants::EPSILON) > l.max_y()) {
				return false;
		}
		const units::Coordinate denom = l.end.x - l.start.x;
		if (std::abs(denom) < constants::EPSILON) {
			// Vertical line. Bounding box check guarantees intersection.
			return true;
		}
		const units::Coordinate numer = l.end.y - l.start.y;
		if (std::abs(numer) < constants::EPSILON) {
			// Horizontal line. Bounding box check guarantees intersection.
			return true;
		}
		// Some diagonal line. Apply line equation to test the point.
		// We've already bounds checked, and checked for division by zero.
		const units::Coordinate slope = numer/denom;
		const units::Coordinate intercept = l.start.y - slope*l.start.x; // b = y - mx
		const units::Coordinate q = slope*p.x + intercept; // y = mx + b
		// Check with line equation.
		return std::abs(q - p.y) < constants::EPSILON;
	}

	bool intersects(const Ray& r, const units::Coordinate2D p) {
		// Bounds test. Point must be either at origin, or away from the origin in the direction of the ray.
		if ((r.dir.y >= 0 ? ((p.y + constants::EPSILON) < r.origin.y) : ((p.y - constants::EPSILON) > r.origin.y)) ||
			(r.dir.x >= 0 ? ((p.x + constants::EPSILON) < r.origin.x) : ((p.x - constants::EPSILON) > r.origin.x))) {
				return false;
		}
		if (std::abs(r.dir.x) < constants::EPSILON) {
			// Vertical ray.
			return std::abs(p.x - r.origin.x) < constants::EPSILON && 
				(r.dir.y > 0 ? (p.y > r.origin.y) : (p.y < r.origin.y) );
		}
		if (std::abs(r.dir.y) < constants::EPSILON) {
			// Horizontal ray.
			return std::abs(p.y - r.origin.y) < constants::EPSILON && 
				(r.dir.x > 0 ? (p.x > r.origin.x) : (p.x < r.origin.x) );
		}
		// Check for a point on the line created by the ray.
		// We've already bounds checked, and checked for division by zero.
		const units::Coordinate slope = r.dir.y / r.dir.x;
		const units::Coordinate intercept = r.origin.y - slope*r.origin.x; // b = y - mx
		const units::Coordinate q = slope*p.x + intercept; // y = mx + b
		return std::abs(q - p.y) < constants::EPSILON; 
	}

	// ---------------------------- No output point intersections --------------------------------------------

	bool intersects(const LineSegment& a, const LineSegment& b) {
		char d1 = _compute_direction(b.start, b.end, a.start);
		char d2 = _compute_direction(b.start, b.end, a.end);
		char d3 = _compute_direction(a.start, a.end, b.start);
		char d4 = _compute_direction(a.start, a.end, b.end);
		return (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) ||
			(d1 == 0 && _is_on_segment(b.start, b.end, a.start)) ||
			(d2 == 0 && _is_on_segment(b.start, b.end, a.end)) ||
			(d3 == 0 && _is_on_segment(a.start, a.end, b.start)) ||
			(d4 == 0 && _is_on_segment(a.start, a.end, b.end));
	}



	// --------------------------- Intersections with output point ----------------------------------------------------

	bool intersects(const LineSegment& a, const LineSegment& b, units::Coordinate2D& out_intersection) {
		// Check if either or both segments are a point.
		if (a.isPoint()) {
			if (util::almostEquals(a.start.x, b.start.x) && util::almostEquals(a.start.y, b.start.y)) {
				// This catches the case where o is also a point.
				out_intersection = a.start;
				return true;
			}
			if (intersects(b, a.start)) {
				out_intersection = a.start;
				return true;
			}
			return false;
		}
		if (b.isPoint()) {
			if (intersects(a, b.start)) {
				out_intersection = b.start;
				return true;
			}
			return false;
		}
		const units::Coordinate2D r = a.end - a.start;
		const units::Coordinate2D s = b.end - b.start;
		const units::Coordinate2D qp = b.start - a.start;
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
					out_intersection = a.start + t0*r;
					return true;
				}
			} else {
				if (0 <= t1 && t0 <= 1) {
					// Get closest intersect (to start point) for the segment this is called on.
					out_intersection = a.start + t1*r;
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
			out_intersection = a.start + t*r;
			return true;
		}
		// No intersection.
		return false;
	}
	bool intersects(const Ray& r, const LineSegment& l, units::Coordinate2D& out_intersection) {
		// Check if the line segment is really a point.
		if (l.isPoint()) {
			if (intersects(r, l.start)) {
				out_intersection = l.start;
				return true;
			}
			return false;
		}
		const units::Coordinate2D s = l.end - l.start;
		const units::Coordinate2D qp = l.start - r.origin;
		const units::Coordinate rxs = r.dir.cross(s);
		const units::Coordinate qpxr = qp.cross(r.dir);
		if (rxs == 0 && qpxr == 0) {
			// Lines are colinear. Test for overlap.
			const units::Coordinate recip = 1/r.dir.dot(r.dir); // Save a division.
			const units::Coordinate t0 = qp.dot(r.dir) * recip;
			const units::Coordinate s_dot_r = s.dot(r.dir);
			// See if the lines overlap.
			if (s_dot_r < 0) {
				if (0 <= t0) {
					// Get closest intersect (to origin) for the ray.
					out_intersection = r.origin + (t0 + s_dot_r * recip)*r.dir;
					return true;
				}
			} else {
				if (0 <= (t0 + s_dot_r * recip)) {
					// Get closest intersect (to origin) for the ray.
					out_intersection = r.origin + t0*r.dir;
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
		if (0 <= t && 0 <= u && u <= 1) {
			// They intersect.
			out_intersection = r.origin + t*r.dir;
			return true;
		}
		// No intersection.
		return false;
	}
}