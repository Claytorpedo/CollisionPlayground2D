#include "intersections.hpp"

#include "units.hpp"
#include "math.hpp"
#include "constants.hpp"
#include "debug_logger.hpp"

#include "LineSegment.hpp"
#include "Ray.hpp"
#include "Shape.hpp"
#include "ShapeContainer.hpp"
#include "Rectangle.hpp"
#include "Projection.hpp"

namespace geom {
	// ------------------------------- Point intersections --------------------------------------------------

	bool intersects(const Rect& r, const Coord2& p) {
		return (p.x >= (r.left()   - constants::EPSILON) &&
		        p.x <= (r.right()  + constants::EPSILON) &&
		        p.y >= (r.top()    - constants::EPSILON) &&
		        p.y <= (r.bottom() + constants::EPSILON));
	}
	bool intersects(const LineSegment& l, const Coord2& p) {
		// Check bounding box.
		if ((p.x + constants::EPSILON) < l.min_x() || (p.x - constants::EPSILON) > l.max_x() ||
			(p.y + constants::EPSILON) < l.min_y() || (p.y - constants::EPSILON) > l.max_y()) {
				return false;
		}
		const gFloat denom = l.end.x - l.start.x;
		if (std::abs(denom) < constants::EPSILON) {
			// Vertical line. Bounding box check guarantees intersection.
			return true;
		}
		const gFloat numer = l.end.y - l.start.y;
		if (std::abs(numer) < constants::EPSILON) {
			// Horizontal line. Bounding box check guarantees intersection.
			return true;
		}
		// Some diagonal line. Apply line equation to test the point.
		// We've already bounds checked, and checked for division by zero.
		const gFloat slope = numer/denom;
		const gFloat intercept = l.start.y - slope*l.start.x; // b = y - mx
		const gFloat q = slope*p.x + intercept; // y = mx + b
		// Check with line equation.
		return std::abs(q - p.y) < constants::EPSILON;
	}

	bool intersects(const Ray& r, const Coord2& p) {
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
		const gFloat slope = r.dir.y / r.dir.x;
		const gFloat intercept = r.origin.y - slope*r.origin.x; // b = y - mx
		const gFloat q = slope*p.x + intercept; // y = mx + b
		return std::abs(q - p.y) < constants::EPSILON; 
	}

	// ---------------------------- No output point intersections --------------------------------------------

	namespace {
		inline char _compute_direction(const Coord2& a, const Coord2& b, const Coord2& c ) {
			const gFloat p = (c.x - a.x) * (b.y - a.y);
			const gFloat q = (b.x - a.x) * (c.y - a.y);
			return p < q ? -1 : p > q ? 1 : 0;
		}
		inline bool _is_on_segment(const Coord2& a, const Coord2& b, const Coord2& c) {
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
	// This is essentially just as fast as the output point version.
	bool intersects_ignore_parallel(const Ray& r, const LineSegment& l) {
		// Bounds test. Either start or end of line must be either at the ray's origin, or past the ray's origin in the ray's direction.
		if ((r.dir.y >= 0 ? (l.start.y < r.origin.y && l.end.y < r.origin.y) : (l.start.y > r.origin.y && l.end.y > r.origin.y)) ||
			(r.dir.x >= 0 ? (l.start.x < r.origin.x && l.end.x < r.origin.x) : (l.start.x > r.origin.x && l.end.x > r.origin.x)))
			return false;

		const Coord2 s = l.end - l.start;
		const gFloat rxs = r.dir.cross(s);
		if (math::almostZero(rxs))
			return false; // Parallel lines.
		const Coord2 qp = l.start - r.origin;
		const Coord2 axis(r.dir.perpCCW());
		//     [is in front of ray]             [is on the line segment]
		return 0 <= qp.cross(s)*rxs && math::isBetween(axis.dot(r.origin), axis.dot(l.start), axis.dot(l.end));
	}

	// --------------------------- Intersections with output point ----------------------------------------------------

	bool intersects(const LineSegment& a, const LineSegment& b, Coord2& out_intersection) {
		// Bounds test for early out.
		if (a.min_x() > b.max_x() || a.max_x() < b.min_x() || a.min_y() > b.max_y() || a.max_y() < b.min_y())
			return false;

		// Check if either or both segments are a point.
		if (a.isPoint()) {
			if (math::almostEquals(a.start.x, b.start.x) && math::almostEquals(a.start.y, b.start.y)) {
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
		const Coord2 r = a.end - a.start;
		const Coord2 s = b.end - b.start;
		const Coord2 qp = b.start - a.start;
		const gFloat rxs = r.cross(s);
		const gFloat qpxr = qp.cross(r);
		if (rxs == 0) {
			if (qpxr != 0)
				return false; // Parallel and non-intersecting.
			// Lines are colinear. Test for overlap.
			// Don't have to worry about r being zero, since we tested for points already.
			const gFloat r2 = r.magnitude2();
			const gFloat t0 = qp.dot(r) / r2;
			const gFloat s_dot_r = s.dot(r);
			const gFloat t1 = t0 + s_dot_r / r2;
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
		const gFloat t = qp.cross(s) / rxs;
		const gFloat u = qpxr / rxs;
		if (0 <= t && t <= 1 && 0 <= u && u <= 1) {
			out_intersection = a.start + t*r;
			return true;
		}
		return false;
	}
	bool intersects(const Ray& r, const LineSegment& l, gFloat& out_t) {
		// Bounds test. Either start or end of line must be either at the ray's origin, or past the ray's origin in the ray's direction.
		if ((r.dir.y >= 0 ? (l.start.y < r.origin.y && l.end.y < r.origin.y) : (l.start.y > r.origin.y && l.end.y > r.origin.y)) ||
		    (r.dir.x >= 0 ? (l.start.x < r.origin.x && l.end.x < r.origin.x) : (l.start.x > r.origin.x && l.end.x > r.origin.x)) )
			return false;

		const Coord2 s = l.end - l.start;
		const Coord2 qp = l.start - r.origin;
		const gFloat rxs = r.dir.cross(s);
		const gFloat qpxr = qp.cross(r.dir);
		if (rxs == 0) {
			if (qpxr != 0)
				return false; // Parallel and non-intersecting.
			// They are colinear. Test for overlap.
			const gFloat r2 = r.dir.magnitude2();
			const gFloat t0 = qp.dot(r.dir) / r2;
			const gFloat s_dot_r = s.dot(r.dir);
			if (s_dot_r < 0) {
				if (0 <= t0) {
					// Get closest intersept (to origin) for the ray.
					// Either the interval of overlap happens somewhere after origin, or at origin.
					const gFloat t1 = t0 + s_dot_r / r2;
					out_t = t1 > 0.0f ? t1: 0.0f;
					return true;
				}
			} else {
				if (0 <= (t0 + s_dot_r / r2)) {
					// Get closest intersect (to origin) for the ray.
					// Either the interval of overlap happens somewhere after origin, or at origin.
					out_t = t0 > 0.0f ? t0 : 0.0f;
					return true;
				}
			}
			return false; // Colinear with no overlap.
		}
		// Guaranteed that rxs != 0 here: they are not parallel.
		const gFloat t = qp.cross(s) / rxs;
		const gFloat u = qpxr / rxs;
		if (0 <= t && 0 <= u && u <= 1) {
			out_t = t;
			return true;
		}
		return false;
	}
	bool intersects_ignore_parallel(const Ray& r, const LineSegment& l, gFloat& out_t) {
		// Bounds test. Either start or end of line must be either at the ray's origin, or past the ray's origin in the ray's direction.
		if ((r.dir.y >= 0 ? (l.start.y < r.origin.y && l.end.y < r.origin.y) : (l.start.y > r.origin.y && l.end.y > r.origin.y)) ||
			(r.dir.x >= 0 ? (l.start.x < r.origin.x && l.end.x < r.origin.x) : (l.start.x > r.origin.x && l.end.x > r.origin.x)))
			return false;

		const Coord2 s = l.end - l.start;
		const gFloat rxs = r.dir.cross(s);
		if (math::almostZero(rxs))
			return false; // Parallel lines.
		const Coord2 qp = l.start - r.origin;
		const gFloat t = qp.cross(s) / rxs;
		const gFloat u = qp.cross(r.dir) / rxs;
		if (0 <= t && 0 <= u && u <= 1) { // In front of ray, and on line segment.
			out_t = t;
			return true;
		}
		return false;
	}

	// ------------------------------- Shape/primative intersections ----------------------------------------
	bool intersects(const Rect& r, const LineSegment& l) {
		// Bounds test for early out.
		if (r.left() > l.max_x() || r.right() < l.min_x() || r.top() > l.max_y() || r.bottom() < l.min_y())
			return false;
		// Check if either endpoints are inside/touching the rectangle.
		if (intersects(r, l.start) || intersects(r, l.end))
			return true;
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

	// ------------------------------- Ray and rectangle intersections ----------------------------------------
	bool intersects(const Ray& ray, const Rect& rect) {
		if ((ray.dir.x == 0 || (ray.dir.x > 0 ? rect.right()  < ray.origin.x : rect.left() > ray.origin.x)) &&
			(ray.dir.y == 0 || (ray.dir.y > 0 ? rect.bottom() < ray.origin.y : rect.top()  > ray.origin.y)))
			return false; // The rectangle is behind the ray.
		if (intersects(rect, ray.origin))
			return true; // The ray's origin is in the rectangle.
		if (ray.dir.x != 0.0f) {
			if (intersects_ignore_parallel(ray, ray.dir.x > 0.0f ? LineSegment(rect.topLeft(), rect.bottomLeft()) : LineSegment(rect.topRight(), rect.bottomRight())))
				return true;
		}
		if (ray.dir.y != 0.0f) {
			if (intersects_ignore_parallel(ray, ray.dir.x > 0.0f ? LineSegment(rect.bottomLeft(), rect.bottomRight()) : LineSegment(rect.topLeft(), rect.topRight())))
				return true;
		}
		return false;
	}
	bool intersects(const Ray& ray, const Rect& rect, const Coord2& pos) {
		return intersects(ray, rect + pos);
	}
	bool intersects(const Ray& ray, const Rect& rect, const Coord2& pos, gFloat& out_t) {
		if ((ray.dir.x == 0 || (ray.dir.x > 0 ? rect.right()  < ray.origin.x : rect.left() > ray.origin.x)) &&
			(ray.dir.y == 0 || (ray.dir.y > 0 ? rect.bottom() < ray.origin.y : rect.top()  > ray.origin.y)))
			return false; // The rectangle is behind the ray.
		if (intersects(rect, ray.origin)) {
			out_t = 0;
			return true; // The ray's origin is in the rectangle.
		}
		if (ray.dir.x != 0.0f) {
			if (intersects_ignore_parallel(ray, ray.dir.x > 0.0f ? LineSegment(rect.topLeft(), rect.bottomLeft()) : LineSegment(rect.topRight(), rect.bottomRight()), out_t))
				return true;
		}
		if (ray.dir.y != 0.0f) {
			if (intersects_ignore_parallel(ray, ray.dir.x > 0.0f ? LineSegment(rect.bottomLeft(), rect.bottomRight()) : LineSegment(rect.topLeft(), rect.topRight()), out_t))
				return true;
		}
		return false;
	}
	// --------------------------------- Ray and polygon intersections -----------------------------------------
	bool intersects(const Ray& r, const Polygon& p, const Coord2& pos) {
		if ((r.dir.x == 0 || (r.dir.x > 0 ? pos.x + p.right()  < r.origin.x : pos.x + p.left() > r.origin.x)) &&
			(r.dir.y == 0 || (r.dir.y > 0 ? pos.y + p.bottom() < r.origin.y : pos.y + p.top()  > r.origin.y)))
			return false; // The bounding box for the polygon is behind the ray.
		for (std::size_t k = p.size() - 1, i = 0; i < p.size(); k = i++) {
			if (intersects_ignore_parallel(r, LineSegment(pos + p[k], pos + p[i])))
				return true;
		}
		return false;
	}
	// --------------------------------- Ray and circle intersections ------------------------------------------
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos) {
		if (math::closestDistToLine(r, pos + c.center) > c.radius)
			return false; // Ray passes the circle.
		const Coord2 originToCircle(pos + c.center - r.origin);
		if (originToCircle.magnitude2() <= c.radius*c.radius)
			return true; // The ray's origin is inside the circle.
		return originToCircle.dot(r.dir) >= 0.0f; // Check if the circle is in front of the ray.
	}
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_t) {
		const Coord2 originToCircle(c.center + pos - r.origin);
		const gFloat r2(c.radius*c.radius);
		const gFloat overlap(originToCircle.magnitude2() - r2);
		if (overlap <= 0.0f) {
			out_t = 0;
			return true; // The ray's origin is inside the circle.
		}
		const gFloat proj(r.dir.dot(originToCircle));
		if (proj < 0.0f)
			return false; // Circle is behind the ray.
		const gFloat d(proj*proj - overlap); // Solve quadratic.
		if (d < 0)
			return false; // Ray misses.
		out_t = proj - std::sqrt(d);
		return true;
	}
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_t, Coord2& out_norm) {
		if (!intersects(r, c, pos, out_t))
			return false;
		out_norm = out_t == 0.0f ? Coord2(0, 0) : ((r.origin + r.dir*out_t) - (c.center + pos)).normalize();
		return true;
	}
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_enter, gFloat& out_exit) {
		const Coord2 originToCircle(c.center + pos - r.origin);
		const gFloat proj(r.dir.dot(originToCircle));
		if (proj < -c.radius)
			return false; // Circle is behind the ray.
		const gFloat overlap(originToCircle.magnitude2() - c.radius*c.radius);
		const gFloat d(proj*proj - overlap); // Solve quadratic.
		if (d < 0)
			return false; // Ray misses.
		const gFloat sqrt_d(std::sqrt(d));
		const gFloat t(proj - sqrt_d);
		if (t < 0) { // First intersect is behind the ray. Check if the ray's origin is inside the circle.
			const gFloat t2(proj + sqrt_d);
			if (t2 < 0)
				return false; // Circle is behind the ray.
			// Ray starts inside the circle.
			out_enter = 0.0f;
			out_exit = t2;
			return true;
		}
		out_enter = t;
		out_exit = proj + sqrt_d;
		return true;
	}
	bool intersects(const Ray& r, const Circle& c, const Coord2& pos, gFloat& out_enter, Coord2& out_norm_enter, gFloat& out_exit, Coord2& out_norm_exit) {
		if (!intersects(r, c, pos, out_enter, out_exit))
			return false;
		out_norm_enter = out_enter == 0.0f ? Coord2(0, 0) : ((r.origin + r.dir*out_enter) - (c.center + pos)).normalize();
		out_norm_exit = ((r.origin + r.dir*out_exit) - (c.center + pos)).normalize();
		return true;
	}
	// ----------------------------- Ray and shape container intersections --------------------------------------
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos);
		}
	}/*
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_t) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos, out_t);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos, out_t);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos, out_t);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos, out_t);
		}
	}
	bool intersects(const Ray& r, const ShapeContainer& s, const Coord2& pos, gFloat& out_enter, gFloat& out_exit) {
		switch (s.type()) {
		case ShapeType::RECTANGLE: return intersects(r, s.rect(), pos, out_enter, out_exit);
		case ShapeType::POLYGON:   return intersects(r, s.poly(), pos, out_enter, out_exit);
		case ShapeType::CIRCLE:    return intersects(r, s.circle(), pos, out_enter, out_exit);
		default:
			DBG_WARN("Unhandled shape type for ray-shape intersection. Converting to polygon.");
			return intersects(r, s.shape().toPoly(), pos, out_enter, out_exit);
		}
	}*/
}
