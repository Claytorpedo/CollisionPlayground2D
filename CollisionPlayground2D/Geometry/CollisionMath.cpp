#include "CollisionMath.h"

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
		return (p.x >= r.left()   - constants::EPSILON && 
			p.x <= r.right()  + constants::EPSILON && 
			p.y >= r.bottom() - constants::EPSILON && 
			p.y <= r.top()    + constants::EPSILON);
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

	// ------------------------------- Shape intersections --------------------------------------------------

	bool intersects(const Rectangle& first, const Rectangle& second) {
		return first.left()   < second.right()  &&
			first.right()  > second.left()   &&
			first.top()    < second.bottom() &&
			first.bottom() > second.top();
	}
	bool intersects(const Rectangle& r, const LineSegment& l) {
		// Bounds test for early out.
		if (!intersects(r, Rectangle(l.min_x(), l.min_y(), l.max_x() - l.min_x(), l.max_y() - l.min_y())))
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


namespace collision_math {
	bool collides(const Polygon& collider, const units::Coordinate2D dir, const units::Coordinate delta, const Polygon& other) {
		return isect::intersects(collider.clipExtend(dir, delta), other);
	}

	namespace {
		enum NormalType { // The type of normal being looked for.
			INVALID,
			VERTEX,
			EDGE
		};
		// Info about depth tests.
		struct DepthTestInfo {
			NormalType type;     // The type of normal for the collision (vertex or edge).
			std::size_t index;   // The vertex or edge tested on (on the other polygon). If edge, then the edge is made from (index-1, index).
			units::Coordinate depthSquared; // The squared depth of the test.
			DepthTestInfo() : type(INVALID), index(0), depthSquared(0) {}
			DepthTestInfo(NormalType type, std::size_t index, units::Coordinate depthSquared) : type(type), index(index), depthSquared(depthSquared) {}
			inline bool isValid() const { return type != INVALID; }
		};
		// Trace rays from the relevant vertices in the other polygon towards the relevant edges of the clipped collider.
		// firstInRange and lastInRange are the results from findExtendRange() on the other polygon using the opposite of the delta direction.
		// Return the result that has the longest/deepest ray, if one exists. Will be testing for vertex collisions.
		inline DepthTestInfo _deepest_dist_to_vertices(const Polygon& clippedCollider, const Polygon& other, 
			const std::size_t firstInRange, const std::size_t lastInRange, const units::Coordinate2D& dir) {
			DepthTestInfo deepest = DepthTestInfo();
			const std::size_t otherSize = other.size();
			const std::size_t clippedSize = clippedCollider.size();
			bool foundRange = false; // Whether we have started finding ray intersections (any distance found).
			for (std::size_t i = firstInRange; ; ++i) {
				if (i == otherSize) // Wrap around the polygon's vertices, in the case that first > last.
					i = 0;
				bool found = false;
				// Need to shoot rays towards the inside of the other polygon (itself), so use delta direction.
				Ray r(other[i], dir);

				for (std::size_t k = 2; k < clippedSize - 1; ++k) { // A clipped and extended poly has at least 5 vertices.
					units::Coordinate2D out_point;
					if (isect::intersects(r, LineSegment(clippedCollider[k-1], clippedCollider[k]), out_point)) {
						const units::Coordinate mag2 = (other[i] - out_point).magnitude2();
						if (!deepest.isValid()) {
							deepest = DepthTestInfo(VERTEX, i, mag2); // First time finding one.
							foundRange = true;
						} else if (deepest.depthSquared < mag2) {
							deepest = DepthTestInfo(VERTEX, i, mag2); // Found a better one.
						} else {
							return deepest; // The distance has started decreasing (or stayed the same, and therefore can only decrease from here).
						}
						found = true; // Mark that we found something.
						break; // Each delta ray from a vertex will intersect at most one edge of the collider.
					}
				}
				// Check for an early exit case. If we have found a match previously, and now stopped, that indicates there will be no further matches.
				if (foundRange && !found)
					break; // We stopped finding intersections with our ray test. Can early exit if there are more vertices after this point.
				if (i == lastInRange) // Exit condition; checked all vertices.
					break;
			}
			return deepest;
		}

		// Trace rays from the relevant vertices in the clipped collider towards the relevant edges of the other polygon.
		// firstInRange and lastInRange are the results from findExtendRange() on the other polygon using the opposite of the delta direction.
		// Return the result that has the longest/deepest ray, if one exists. Will be testing for edge collisions.
		inline DepthTestInfo _deepest_dist_to_edges(const Polygon& clippedCollider, const Polygon& other, 
			const std::size_t firstInRange, const std::size_t lastInRange, const units::Coordinate2D& oppDir) {
			DepthTestInfo deepest = DepthTestInfo();
			const std::size_t otherSize = other.size();
			const std::size_t clippedSize = clippedCollider.size();
			bool foundRange = false; // Whether we have started finding ray intersections (any distance found).
			// Loop through relevant vertices on the clipped collider, testing against relevant edges on the other polygon.
			for (std::size_t i = 1; i < clippedSize - 1; ++i) { // Ignore the vertices that weren't extenced.
				bool found = false;
				// Shoot rays towards the inside of the clipped collider (itself), so use opposite of the delta direction.
				Ray r(clippedCollider[i], oppDir);

				for (std::size_t k = firstInRange + 1; ; ++k) {
					if (k == otherSize) // Wrap around the polygon's vertices, in the case that first > last.
						k = 0;
					units::Coordinate2D out_point;
					// Get the edge, watching for the case where the edge branches over the end of the vertex list.
					if (isect::intersects(r, LineSegment(other[k == 0 ? otherSize-1 : k-1], other[k]), out_point)) {
						const units::Coordinate mag2 = (clippedCollider[i] - out_point).magnitude2();
						if (!deepest.isValid()) {
							deepest = DepthTestInfo(EDGE, k, mag2); // First time finding one.
							foundRange = true;
						} else if (deepest.depthSquared < mag2) {
							deepest = DepthTestInfo(EDGE, k, mag2); // Found a better one.
						} else {
							return deepest; // The distance has started decreasing (or stayed the same, and therefore can only decrease from here).
						}
						found = true; // Mark that we found something.
						break; // Each delta ray from a vertex will intersect at most one edge of the other polygon.
					}
					if (k == lastInRange) // Exit condition; checked all edges.
						break;
				}
				// Check for an early exit case. If we have found a match previously, and now stopped, that indicates there will be no further matches.
				if (foundRange && !found)
					break; // We stopped finding intersections with our ray test. Can early exit if there are more vertices after this point.
			}
			return deepest;
		}

		// Find how much the colliding polygon penetrated the other polygon along the delta vector.
		// In doing so, also determine the normal of the vertex or edge that it entered through.
		inline bool _find_max_penetration(const Polygon& clippedCollider, const units::Coordinate2D& dir, const units::Coordinate delta,
			const Polygon& other, units::Coordinate2D& out_normal, units::Coordinate& out_delta) {
			const units::Coordinate2D oppDir(dir.neg()); // Delta in the opposite direction.
			// Find the range of vertices (and edges) that the collider may have entered from.
			std::size_t firstInRange, lastInRange;
			bool a,b; // Don't care about these.
			if ( !other.findExtendRange(oppDir, firstInRange, lastInRange, a, b) ) {
				return false; // The second polygon isn't valid (ideally this should never happen: most invalid polygons should be caught by the SAT test).
			}

			DepthTestInfo testVertices = _deepest_dist_to_vertices(clippedCollider, other, firstInRange, lastInRange, dir);
			DepthTestInfo testEdges = _deepest_dist_to_edges(clippedCollider, other, firstInRange, lastInRange, oppDir);
			DepthTestInfo deepest;
			if (!testVertices.isValid() && !testEdges.isValid()) {
				std::cerr << "Error: Could not find any penetration depth. (Are the polygons really colliding?)\n";
				std::cerr << dir.x << "," << dir.y << " delta: " << delta << " combDelta: " << (dir*delta).x << "," << (dir*delta).y << "\n";
				std::cerr << "intersects? " << (isect::intersects(clippedCollider, other) ? "true\n" : "false\n");
				out_normal = units::Coordinate2D(0,0);
				out_delta = 0;

				return false;
			}
			if (!testVertices.isValid()) {
				deepest = testEdges;
			} else if (!testEdges.isValid()) {
				deepest = testVertices;
			} else {
				// Be biased towards choosing vertex collisions, since it is possible for a vertex collision to also register as an edge collision.
				// (And if a collision is extremely close to a vertex, it shouldn't be problematic to use the vertex instead.)
				deepest = testVertices.depthSquared >= testEdges.depthSquared ? testVertices : testEdges;
			}
			 // How far to push out of the polygon so that it is long longer a collision.
			const units::Coordinate pushOut(std::sqrt(deepest.depthSquared) + collision_math::COLLISION_PUSHOUT_DISTANCE);
			const units::Coordinate newDelta(delta - pushOut);
			out_delta = newDelta > 0.0f ? newDelta : 0.0f; // Avoid pushing the collider further back than where it started.

			// Find the normal.
			if (deepest.type == EDGE) {
				// We only need to compute for the single edge, taking into account the fact that it might wrap.
				const units::Coordinate2D first(deepest.index == 0 ? other[other.size()-1] : other[deepest.index - 1]);
				const units::Coordinate2D second(other[deepest.index]);
				out_normal = units::Coordinate2D(first.y - second.y, second.x - first.x).normalize();

			} else { // Vertex normal.
				// Compute the vertex normal by combining and normalizing the two connecting edge normals.
				const units::Coordinate2D first(deepest.index == 0 ? other[other.size()-1] : other[deepest.index - 1]);
				const units::Coordinate2D second(other[deepest.index]);
				const units::Coordinate2D third((deepest.index + 1) == other.size() ? other[0] : other[deepest.index + 1]);
				// Get both edge normals.
				const units::Coordinate2D edge1 = units::Coordinate2D(first.y - second.y, second.x - first.x).normalize();
				const units::Coordinate2D edge2 = units::Coordinate2D(second.y - third.y, third.x - second.x).normalize();
				// Get vertex normal.
				out_normal = (edge1 + edge2).normalize();
			}
			return true;
		}
	}

	// Test for collision, and if they collide find the collision normal and how far along delta can be travelled.
	bool collides(const Polygon& collider, const units::Coordinate2D& dir, const units::Coordinate delta,
		const Polygon& other, units::Coordinate2D& out_normal, units::Coordinate& out_delta) {
		if (dir.isZero() || delta == 0.0f)
			return false; // Should not call this function with zero delta.
		Polygon clippedCollider(collider.clipExtend(dir, delta));
		if ( !isect::intersects(clippedCollider, other) )
			return false;
		_find_max_penetration(clippedCollider, dir, delta, other, out_normal, out_delta);
		return true;
	}
}