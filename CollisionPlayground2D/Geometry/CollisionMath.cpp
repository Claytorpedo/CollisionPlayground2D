#include "CollisionMath.h"

#include <iostream>

#include "../Units.h"

#include "IntersectionMath.h"
#include "LineSegment.h"
#include "Ray.h"
#include "Polygon.h"

namespace collision_math {
	bool collides(const Polygon& collider, const units::Coordinate2D& dir, const units::Coordinate dist, const Polygon& other) {
		return clippedCollides(collider.clipExtend(dir, dist), other);
	}
	bool clippedCollides(const Polygon& clippedCollider, const Polygon& other) {
		return isect::intersects(clippedCollider, other);
	}

	namespace {
		// Info about depth tests.
		struct DepthTestInfo {
			bool isValid;                   // Whether the the over values are valid or not.
			units::Coordinate2D edge;	    // The vector that makes up the edge that should be deflected along.
			units::Coordinate depthSquared; // The squared depth of the test.
			DepthTestInfo() : isValid(false), edge(), depthSquared(0) {}
			DepthTestInfo(const units::Coordinate2D& edge, units::Coordinate depthSquared) : isValid(true), edge(edge), depthSquared(depthSquared) {}
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
				bool found = false; // Whether we found an intersection for this vertex.
				// Need to shoot rays towards the inside of the other polygon (itself), so use delta direction.
				const Ray r(other[i], dir);

				for (std::size_t k = 2; k < clippedSize - 1; ++k) { // A clipped and extended poly has at least 5 vertices.
					units::Coordinate2D out_point;
					const LineSegment edge(clippedCollider[k-1], clippedCollider[k]); // Don't have to worry about wrap-around.
					if (isect::intersects(r, edge, out_point)) {
						const units::Coordinate mag2 = (other[i] - out_point).magnitude2();
						if (!deepest.isValid) {
							deepest = DepthTestInfo(edge.end - edge.start, mag2); // First time finding one.
							foundRange = true;
						} else if (deepest.depthSquared < mag2) {
							deepest = DepthTestInfo(edge.end - edge.start, mag2); // Found a better one.
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
				bool found = false; // Whether we found an intersection for this vertex.
				// Shoot rays towards the inside of the clipped collider (itself), so use opposite of the delta direction.
				Ray r(clippedCollider[i], oppDir);

				for (std::size_t k = firstInRange + 1; ; ++k) {
					if (k == otherSize) // Wrap around the polygon's vertices, in the case that first > last.
						k = 0;
					units::Coordinate2D out_point;
					// Get the edge, watching for the case where the edge branches over the end of the vertex list.
					const LineSegment edge(other[k == 0 ? otherSize-1 : k-1], other[k]);
					if (isect::intersects(r, edge, out_point)) {
						const units::Coordinate mag2 = (clippedCollider[i] - out_point).magnitude2();
						if (!deepest.isValid) {
							deepest = DepthTestInfo(edge.end - edge.start, mag2); // First time finding one.
							foundRange = true;
						} else if (deepest.depthSquared < mag2) {
							deepest = DepthTestInfo(edge.end - edge.start, mag2); // Found a better one.
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
		// In doing so, also determine the edge to deflect along.
		inline bool _find_max_penetration(const Polygon& clippedCollider, const units::Coordinate2D& dir, const units::Coordinate dist,
			const Polygon& other, units::Coordinate& out_dist, units::Coordinate2D& out_edge) {
			const units::Coordinate2D oppDir(dir.neg()); // Delta in the opposite direction.
			// Find the range of vertices (and edges) that the collider may have entered from.
			std::size_t firstInRange, lastInRange;
			bool a,b; // Don't care about these.
			if ( !other.findExtendRange(oppDir, firstInRange, lastInRange, a, b) ) {
				std::cerr << "Failed to find max penetration distance, due to invalid polygons.\n";
				out_dist = 0;
				out_edge = units::Coordinate2D(0,0);
				return false;
			}

			DepthTestInfo testVertices = _deepest_dist_to_vertices(clippedCollider, other, firstInRange, lastInRange, dir);
			DepthTestInfo testEdges = _deepest_dist_to_edges(clippedCollider, other, firstInRange, lastInRange, oppDir);
			// Test four cases: both tests are invalid, one or the other test is valid, or both tests are valid.
			if (!testVertices.isValid && !testEdges.isValid) {
				// Are the polygons really colliding?
				// This indicates that the SAT test says these polygons collide, but the penetartion test says they aren't penetrating at all.
				// This may happen on rare occasions due to floating point errors. How to handle this may differ by implementation.

				// Avoid the collider getting stuck. Push it back a bit just in case (we don't want to move the collider to a location where the SAT test is true).
				const units::Coordinate newDist(dist - collision_math::COLLISION_PUSHOUT_DISTANCE);
				out_dist = newDist > 0.0f ? newDist : 0.0f; // Avoid pushing the collider further back than where it started.
				out_edge = units::Coordinate2D(0,0);        // There is no edge to deflect along.
				return false; // Still indicate that we didn't actually complete the test succesfully.
			}
			DepthTestInfo deepest;
			if (!testVertices.isValid) {
				deepest = testEdges;
			} else if (!testEdges.isValid) {
				deepest = testVertices;
			} else { // They are both valid.
				// Be slightly biased towards choosing collider entering on a vertex, since that case is possible to also register as entering on an edge.
				// (And if a collision is extremely close to a vertex, it shouldn't be problematic to use the vertex instead.)
				deepest = testVertices.depthSquared >= testEdges.depthSquared ? testVertices : testEdges;
			}
			 // How far to push out of the polygon so that it is no longer a collision.
			const units::Coordinate pushOut(std::sqrt(deepest.depthSquared) + collision_math::COLLISION_PUSHOUT_DISTANCE);
			units::Coordinate newDist(dist - pushOut);
			newDist = newDist > 0.0f ? newDist : 0.0f; // Avoid pushing the collider further back than where it started.

			out_dist = newDist;
			out_edge = deepest.edge;

			return true;
		}
	}

	// Test for collision, and if they collide find the collision normal and how far along direction vector can be travelled.
	bool collides(const Polygon& collider, const units::Coordinate2D& dir, const units::Coordinate dist,
		const Polygon& other, units::Coordinate& out_dist, units::Coordinate2D& out_edge) {
		return clippedCollides(collider.clipExtend(dir, dist), dir, dist, other, out_dist, out_edge);
	}
	// Test for collision with an already clip-extended collider.
	bool clippedCollides(const Polygon& clippedCollider, const units::Coordinate2D& dir, units::Coordinate dist, const Polygon& other,
		units::Coordinate& out_dist, units::Coordinate2D& out_edge) {
		if (dir.isZero() || dist == 0.0f)
			return false; // Should not call this function with zero delta.
		if ( !isect::intersects(clippedCollider, other) )
			return false;
		_find_max_penetration(clippedCollider, dir, dist, other, out_dist, out_edge);
		return true;
	}
}