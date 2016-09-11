#include "Ray.h"

#include "../Constants.h"

#include <cmath>

bool Ray::intersects(const units::Coordinate2D point) const {
	// Bounds test. Point must be either at origin, or away from the 
	// origin in the direction of the ray.
	if ((dir.y >= 0 ? (point.y < origin.y) : (point.y > origin.y)) ||
		(dir.x >= 0 ? (point.x < origin.x) : (point.x > origin.x))) {
		return false;
	}
	// Check for vertical or horizontal rays.
	if (std::abs(dir.x) < constants::EPSILON) {
		// Vertical ray.
		return std::abs(point.x - origin.x) < constants::EPSILON && 
			(dir.y > 0 ? (point.y > origin.y) : (point.y < origin.y) );
	}
	if (std::abs(dir.y) < constants::EPSILON) {
		// Horizontal ray.
		return std::abs(point.y - origin.y) < constants::EPSILON && 
			(dir.x > 0 ? (point.x > origin.x) : (point.x < origin.x) );
	}
	// Check for a point on the line created by the ray.
	// We've already bounds checked, and checked for division by zero.
	const units::Coordinate slope = dir.y / dir.x;
	const units::Coordinate intercept = origin.y - slope*origin.x; // b = y - mx
	const units::Coordinate q = slope*point.x + intercept; // y = mx + b
	return std::abs(q - point.y) < constants::EPSILON; 
}

bool Ray::intersects(const LineSegment& o, units::Coordinate2D& out_intersection) const {
	// Check if the line segment is really a point.
	if (o.isPoint()) {
		if (intersects(o.start)) {
			out_intersection = o.start;
			return true;
		}
		return false;
	}
	// TODO: this is still wrong.
	const units::Coordinate2D s = o.end - o.start;
	const units::Coordinate2D qp = o.start - origin;
	const units::Coordinate rxs = dir.cross(s);
	const units::Coordinate qpxr = qp.cross(dir);
	if (rxs == 0 && qpxr == 0) {
		// Lines are colinear. Test for overlap.
		// Don't have to worry about r being zero, since we tested for points already.
		const units::Coordinate recip = 1/dir.dot(dir); // Save a division.
		const units::Coordinate t0 = qp.dot(dir) * recip;
		const units::Coordinate s_dot_r = s.dot(dir);
		const units::Coordinate t1 = t0 + s_dot_r * recip;
		// See if the lines overlap.
		if (s_dot_r < 0) {
			if (0 <= t0 && t1 <= 1) {
				// Get closest intersect (to start point) for the ray.
				out_intersection = origin + t0*dir;
				return true;
			}
		} else {
			if (0 <= t1 && t0 <= 1) {
				// Get closest intersect (to start point) for the ray.
				out_intersection = origin + t1*dir;
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
		out_intersection = origin + t*dir;
		return true;
	}
	// No intersection.
	return false;
}

bool Ray::intersects(const LineSegment& o) const {
	return false;
}