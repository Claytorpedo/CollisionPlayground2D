#include "IntersectionMath.h"

#include "../Units.h"
#include "../Util.h"
#include "../Constants.h"

#include "LineSegment.h"
#include "Ray.h"
#include "Shape.h"
#include "Rectangle.h"
#include "Polygon.h"

namespace isect {

	// ------------------------------- Point intersections --------------------------------------------------

	bool intersects(const Rectangle& r, const units::Coordinate2D p) {
		return (p.x >= (r.left()   - constants::EPSILON) && 
			    p.x <= (r.right()  + constants::EPSILON) && 
			    p.y >= (r.top()    - constants::EPSILON) && 
			    p.y <= (r.bottom() + constants::EPSILON));
	}
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
				(r.dir.y > 0 ? (p.y >= r.origin.y) : (p.y <= r.origin.y) );
		}
		if (std::abs(r.dir.y) < constants::EPSILON) {
			// Horizontal ray.
			return std::abs(p.y - r.origin.y) < constants::EPSILON && 
				(r.dir.x > 0 ? (p.x >= r.origin.x) : (p.x <= r.origin.x) );
		}
		// Check for a point on the line created by the ray.
		// We've already bounds checked, and checked for division by zero.
		const units::Coordinate slope = r.dir.y / r.dir.x;
		const units::Coordinate intercept = r.origin.y - slope*r.origin.x; // b = y - mx
		const units::Coordinate q = slope*p.x + intercept; // y = mx + b
		return std::abs(q - p.y) < constants::EPSILON; 
	}

	// ---------------------------- No output point intersections --------------------------------------------

	namespace {
		inline char _compute_direction(const units::Coordinate2D& a, const units::Coordinate2D& b, const units::Coordinate2D& c ) {
			const units::Coordinate p = (c.x - a.x) * (b.y - a.y);
			const units::Coordinate q = (b.x - a.x) * (c.y - a.y);
			return p < q ? -1 : p > q ? 1 : 0;
		}
		inline bool _is_on_segment(const units::Coordinate2D& a, const units::Coordinate2D& b, const units::Coordinate2D& c) {
			return	(a.x <= c.x || b.x <= c.x) && (c.x <= a.x || c.x <= b.x) &&
				    (a.y <= c.y || b.y <= c.y) && (c.y <= a.y || c.y <= b.y);
		}
	}
	// Faster test to see if two line segments intersect.
	bool intersects(const LineSegment& a, const LineSegment& b) {
		const char d1 = _compute_direction(b.start, b.end, a.start);
		const char d2 = _compute_direction(b.start, b.end, a.end);
		const char d3 = _compute_direction(a.start, a.end, b.start);
		const char d4 = _compute_direction(a.start, a.end, b.end);
		return (((d1 > 0 && d2 < 0) || (d1 < 0 && d2 > 0)) && ((d3 > 0 && d4 < 0) || (d3 < 0 && d4 > 0))) ||
			(d1 == 0 && _is_on_segment(b.start, b.end, a.start)) ||
			(d2 == 0 && _is_on_segment(b.start, b.end, a.end))   ||
			(d3 == 0 && _is_on_segment(a.start, a.end, b.start)) ||
			(d4 == 0 && _is_on_segment(a.start, a.end, b.end));
	}

	// --------------------------- Intersections with output point ----------------------------------------------------

	bool intersects(const LineSegment& a, const LineSegment& b, units::Coordinate2D& out_intersection) {
		// Bounds test for early out.
		if (a.min_x() > b.max_x() || a.max_x() < b.min_x() || a.min_y() > b.max_y() || a.max_y() < b.min_y())
			return false;

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
		if (rxs == 0) {
			if (qpxr != 0)
				return false; // Parallel and non-intersecting.
			// Lines are colinear. Test for overlap.
			// Don't have to worry about r being zero, since we tested for points already.
			const units::Coordinate r2 = r.magnitude2();
			const units::Coordinate t0 = qp.dot(r) / r2;
			const units::Coordinate s_dot_r = s.dot(r);
			const units::Coordinate t1 = t0 + s_dot_r / r2;
			if (s_dot_r < 0) {
				if (0 <= t0 && t1 <= 1) {
					// Get closest intersect (to start point) for the segment this is called on.
					// Either the interval of overlap happens somewhere after start, or at the starting point.
					out_intersection = t1 > 0.0 ? a.start + t1*r : a.start;
					return true;
				}
			} else {
				if (0 <= t1 && t0 <= 1) {
					// Get closest intersect (to start point) for the segment this is called on.
					// Either the interval of overlap happens somewhere after start, or at the starting point.
					out_intersection = t0 > 0.0 ? a.start + t0*r : a.start;
					return true;
				}
			}			
			return false; // Colinear with no overlap.
		}
		// Guaranteed that rxs != 0 here: lines are not parallel.
		const units::Coordinate t = qp.cross(s) / rxs;
		const units::Coordinate u = qpxr / rxs;
		if (0 <= t && t <= 1 && 0 <= u && u <= 1) {
			out_intersection = a.start + t*r;
			return true;
		}
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
		// Bounds test. Either start or end of line must be either at the ray's origin, or past the ray's origin in the ray's direction.
		if ( (r.dir.y >= 0 ? (l.start.y < r.origin.y && l.end.y < r.origin.y) : (l.start.y > r.origin.y && l.end.y > r.origin.y)) || 
			 (r.dir.x >= 0 ? (l.start.x < r.origin.x && l.end.x < r.origin.x) : (l.start.x > r.origin.x && l.end.x > r.origin.x)) )
				return false;

		const units::Coordinate2D s = l.end - l.start;
		const units::Coordinate2D qp = l.start - r.origin;
		const units::Coordinate rxs = r.dir.cross(s);
		const units::Coordinate qpxr = qp.cross(r.dir);
		if (rxs == 0) {
			if (qpxr != 0)
				return false; // Parallel and non-intersecting.
			// They are colinear. Test for overlap.
			const units::Coordinate r2 = r.dir.magnitude2();
			const units::Coordinate t0 = qp.dot(r.dir) / r2;
			const units::Coordinate s_dot_r = s.dot(r.dir);
			if (s_dot_r < 0) {
				if (0 <= t0) {
					// Get closest intersept (to origin) for the ray.
					// Either the interval of overlap happens somewhere after origin, or at origin.
					const units::Coordinate t1 = t0 + s_dot_r / r2;
					out_intersection = t1 > 0.0f ? r.origin + t1*r.dir : r.origin;
					return true;
				}
			} else {
				if (0 <= (t0 + s_dot_r / r2)) {
					// Get closest intersect (to origin) for the ray.
					// Either the interval of overlap happens somewhere after origin, or at origin.
					out_intersection = t0 > 0.0f ? r.origin + t0*r.dir : r.origin;
					return true;
				}
			}
			return false; // Colinear with no overlap.
		}
		// Guaranteed that rxs != 0 here: they are not parallel.
		const units::Coordinate t = qp.cross(s) / rxs;
		const units::Coordinate u = qpxr / rxs;
		if (0 <= t && 0 <= u && u <= 1) {
			out_intersection = r.origin + t*r.dir;
			return true;
		}
		return false;
	}

	// ------------------------------- Shape/primative intersections ----------------------------------------
	bool intersects(const Rectangle& r, const LineSegment& l) {
		// Check if either endpoints are inside/touching the rectangle.
		if (intersects(r, l.start) || intersects(r, l.end))
			return true;
		// Bounds test for early out.
		if (!intersects(r, Rectangle(l.min_x(), l.min_y(), l.max_x() - l.min_x(), l.max_y() - l.min_y())))
			return false;
		// Test l against 4 line segments that make up the rectangle.
		if (intersects(l, LineSegment(r.left(), r.bottom(), r.left(), r.top())))     // Left side.
			return true;
		if (intersects(l, LineSegment(r.left(), r.top(), r.right(), r.top())))       // Top side.
			return true;
		if (intersects(l, LineSegment(r.right(), r.bottom(), r.right(), r.top())))   // Right side.
			return true;
		if (intersects(l, LineSegment(r.left(), r.bottom(), r.right(), r.bottom()))) // Bottom side.
			return true;
		return false;
	}

	// ------------------------------- Shape intersections --------------------------------------------------

	bool intersects(const Rectangle& first, const Rectangle& second) {
		return first.left()   < second.right()  &&
			   first.right()  > second.left()   &&
			   first.top()    < second.bottom() &&
			   first.bottom() > second.top();
	}

	// ------------------------------ Separating Axis Theorem test ---------------------------------
	namespace {
		struct Projection {
			units::Coordinate min, max;
			Projection() : min(0), max(0) {}
			Projection(units::Coordinate min, units::Coordinate max) : min(min), max(max) {}
		};
		inline Projection _get_projection(const Polygon& poly, units::Coordinate2D axis) {
			units::Coordinate min(poly[0].dot(axis));
			units::Coordinate max(min);
			units::Coordinate proj;
			for (std::size_t i = 1; i < poly.size(); ++i) {
				proj = poly[i].dot(axis);
				if (proj < min)
					min = proj;
				else if (proj > max)
					max = proj;
			}
			return Projection(min, max);
		}
		// Tests the axes of one polygon against the other using SAT.
		inline bool _SAT(const Polygon& first, const Polygon& second) {
			const std::size_t size = first.size();
			units::Coordinate2D axis;
			Projection projFirst, projSecond;
			for (std::size_t i = 0; i < size; ++i) {
				// Find the edge normal. This is the axis we're projecting along.
				const std::size_t next = i+1 >= size ? 0 : i+1;
				axis = units::Coordinate2D(first[i].y - first[next].y, first[next].x - first[i].x);
				// Note that we don't have to normalize the edge normal, since we only care
				// whether or not there is any overlap, not calculating how much there is.

				// Get the projection on of both polygons with the found axis.
				projFirst = _get_projection(first, axis);
				projSecond = _get_projection(second, axis);

				// Test if the projections overlap. If they don't, return false.
				if (projFirst.min > projSecond.max || projFirst.max < projSecond.min)
					return false;
			}
			return true;
		}
		// Perform SAT against both polygons (full test). Returns true on collision.
		inline bool _perform_SAT(const Polygon& first, const Polygon& second) {
			return _SAT(first, second) && _SAT(second, first);
		}
	}
	// ---------------------------------------------------------------------------------------------

	bool intersects(const Polygon& first, const Polygon& second) {
		if (first.isEmpty() || second.isEmpty())
			return false;

		// Bounds test for quick out.
		if (!intersects(Rectangle(first.left(), first.top(), first.right()-first.left(), first.bottom()-first.top()), 
			            Rectangle(second.left(), second.top(), second.right()-second.left(), second.bottom()-second.top())))
			return false;

		return _perform_SAT(first, second);
	}
	bool intersects(const Shape& first, const Shape& second) {
		return intersects(first.toPoly(), second.toPoly());
	}
}