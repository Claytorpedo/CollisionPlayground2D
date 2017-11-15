#include "SAT.h"

#include <vector>

#include "../Units.h"
#include "../Constants.h"
#include "DebugLogger.h"
#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Projection.h"

/*
System for finding the separating axes for the given shapes.
This is a little messy by nature.
Determine the type of shape the first one is, then see if it forms a special case when paired with the second shape.
Returns true if it encounteres a special case that handled both shapes.
*/
inline bool _get_separating_axes(const Shape* const first, const Shape* const second, std::vector<units::Coordinate2D>& axes, bool isFirstCall) {
	if (const Polygon* const p = dynamic_cast<const Polygon* const>(first)) {
		axes.reserve(axes.size() + p->size());
		for (std::size_t i = 0; i < p->size(); ++i)
			axes.push_back(p->getEdgeNorm(i));
	} else if (dynamic_cast<const Rectangle* const>(first)) {
		axes.push_back(units::Coordinate2D(1, 0)); // Rectangles are axis-alligned.
		axes.push_back(units::Coordinate2D(0, 1));
		if (isFirstCall && dynamic_cast<const Rectangle* const>(second)) // Special case: will share axes.
			return true;
	} else {
		DBG_WARN("Unhandled shape type for SAT. Converting to polygon.");
		Polygon p = first->toPoly();
		for (std::size_t i = 0; i < p.size(); ++i)
			axes.push_back(p.getEdgeNorm(i));
	}
	return false;
}
inline std::vector<units::Coordinate2D> _get_separating_axes(const Shape* const first, const Shape* const second) {
	std::vector<units::Coordinate2D> axes;
	if (_get_separating_axes(first, second, axes, true))
		return axes;
	_get_separating_axes(second, first, axes, false);
	return axes;
}

// Tests the axes of one polygon against the other using SAT. Offset is the position of polygon first - second.
// Outputs the normal and distance that make up the minimum translation vector.
inline bool _MTV_SAT(const Polygon& first, const Polygon& second, const units::Coordinate2D& offset, units::Coordinate2D& out_norm, units::Coordinate& out_dist) {
	const std::size_t size = first.size();
	units::Coordinate2D axis, norm(0, 0);
	units::Coordinate overlap1, overlap2, minDist(-1), testDist;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < size; ++i) {
		axis = first.getEdgeNorm(i); // Axis to project along.
		projFirst = first.getProjection(axis);
		projSecond = second.getProjection(axis);
		projFirst += offset.dot(axis); // Apply offset between the two polygons' positions.
		overlap1 = projFirst.max - projSecond.min;
		overlap2 = projSecond.max - projFirst.min;
		if (overlap1 < constants::EPSILON || overlap2 < constants::EPSILON)
			return false;
		// Find separation for this axis (assumes pushing out the first polygon).
		if (projFirst.min < projSecond.min) {
			testDist = overlap1;
			axis = -axis; // Ensure right direction to pushout the first polygon.
		} else {
			testDist = overlap2;
		}
		if (minDist == -1 || testDist < minDist) {
			minDist = testDist;
			norm = axis;
		}
	}
	out_norm = norm;
	out_dist = minDist;
	return true;
}

// Tests the axes of one polygon against the other using SAT. Checks if they are currently overlapping, or will overlap in the future (SAT test and sweep test).
// Note that out_enterTime, out_exitTime, and out_mtv_dist need to be set to defaults on the first call.
// offset   - the position of first - second.
// velocity - the velocity of the first polygon, or firstVel - secondVel (we act as if only first is moving).
// mtv_norm - the normal for a current collision.
// mtv_dist - the distance for the MTV in a current collision.
// out_norm - the normal for future collisions.
// out_enterTime - the first time when all axes overlap. Should be set to < 0 on first call.
// out_exitTime  - the first time after overlap where not all axes overlap anymore. Should be set to the inverval's maximum on first call.
// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
const static units::Fraction MAX_TIME = 1.0f; // Using interval [0,1].
inline sat::HybridResult _hybrid_SAT(const Polygon& first, const Polygon& second, const units::Coordinate2D& offset, const units::Coordinate2D& velocity,
          units::Coordinate2D& out_mtv_norm, units::Coordinate& out_mtv_dist, units::Coordinate2D& out_norm, units::Fraction& out_enterTime, units::Fraction& out_exitTime) {
	bool isOverlapping = out_enterTime < 0.0f; // If we've computed an enter time before, then the shapes are not currently overlapping.
	units::Coordinate testDist, overlap1, overlap2;
	units::Fraction speed, testEnter, testExit;
	units::Coordinate2D axis;
	Projection projFirst, projSecond;
	const std::size_t size = first.size();
	for (std::size_t i = 0; i < size; ++i) {
		axis = first.getEdgeNorm(i);   // Axis to project along.
		projFirst = first.getProjection(axis);
		projSecond = second.getProjection(axis);
		projFirst += offset.dot(axis); // Apply offset between the two polygons' positions.
		overlap1 = projFirst.max - projSecond.min - constants::EPSILON;
		overlap2 = projSecond.max - projFirst.min - constants::EPSILON;
		speed = axis.dot(velocity); // Speed projected along this axis.
		if (overlap1 < 0.0f || overlap2 < 0.0f) { // Not currently overlapping.
			isOverlapping = false;
			if (speed == 0)
				return sat::HybridResult::NONE; // Not moving on this axis (moving parallel, or not at all). They will never meet.
			// Overlaps now tell us how far apart they are on this axis. Divide by speed on this axis to find if/when they will overlap.
			if (overlap1 < 0.0f) { // The projection of the first shape is to the "left" of the second on this axis.
				testEnter = (-overlap1) / speed;
				testExit = overlap2 / speed;
			} else { // Interval on overlap2's side: projection of the first shape is to the "right" of the second on this axis.
				testEnter = overlap2 / speed;
				testExit = (-overlap1) / speed;
			}
			if (testEnter < 0.0f)
				return sat::HybridResult::NONE; // They are moving apart on this axis.
			if (testEnter > out_enterTime) {
				out_enterTime = testEnter; // We want the latest time: the first time when all axes overlap.
				// The last axis to overlap will have the collision normal.
				out_norm = projFirst.min < projSecond.min ? -axis : axis; // Collision normal is relative to the first polygon.
			}
			if (testExit < out_exitTime)
				out_exitTime = testExit; // Keep track of earliest exit time: some axis may stop overlapping before all axes overlap.
			if (out_enterTime > MAX_TIME || out_enterTime > out_exitTime)
				return sat::HybridResult::NONE; // Either don't collide on this time interval, or won't ever with the direction of motion.
		} else { // They are currently overlapping on this axis.
			// Find when the time when they stop overlapping on this axis (start time == 0 == now).
			if (speed != 0) {
				testExit = (speed < 0 ? (-overlap1) : overlap2) / speed;
				if (testExit < out_exitTime)
					out_exitTime = testExit;
				if (out_enterTime > out_exitTime) // There is no interval where all axes have overlap.
					return sat::HybridResult::NONE;
			}
			if (isOverlapping) { // Regular MTV checks.
				// Find separation for this axis (assumes pushing out the first polygon).
				if (projFirst.min < projSecond.min) {
					testDist = overlap1 + constants::EPSILON;
					axis = -axis; // Ensure right direction to push out the first polygon.
				} else {
					testDist = overlap2 + constants::EPSILON;
				}
				if (out_mtv_dist == -1 || testDist < out_mtv_dist) {
					out_mtv_dist = testDist;
					out_mtv_norm = axis;
				}
			}
		}
	}
	return isOverlapping ? sat::HybridResult::MTV : sat::HybridResult::SWEEP;
}
// Performs full hybrid SAT on two polygons.
// Sets all the default values required for the hybird SAT test, and checks the results.
inline sat::HybridResult _perform_hybrid_SAT(const Polygon& first, const Polygon& second, const units::Coordinate2D& offset,
                              const units::Coordinate2D& relativeVel, units::Coordinate2D& out_norm, units::Fraction& out_t) {
	units::Coordinate mtv_dist(-1); // Default value for MTV.
	units::Coordinate2D mtv_norm1(0, 0), mtv_norm2(0, 0), norm1(0, 0), norm2(0, 0);
	units::Fraction enterTime(-1), exitTime(MAX_TIME); // Enter time < 0; max time is the end of the interval.
	if (_hybrid_SAT(first, second, offset, relativeVel, mtv_norm1, mtv_dist, norm1, enterTime, exitTime) == sat::HybridResult::NONE)
		return sat::HybridResult::NONE;
	units::Fraction tempEnter = enterTime;
	units::Coordinate tempDist = mtv_dist;
	sat::HybridResult type = _hybrid_SAT(second, first, -offset, -relativeVel, mtv_norm2, mtv_dist, norm2, enterTime, exitTime);
	if (type == sat::HybridResult::NONE)
		return sat::HybridResult::NONE;
	if (type == sat::HybridResult::MTV) {
		out_norm = tempDist > mtv_dist ? -mtv_norm2 : mtv_norm1;
		out_t = mtv_dist;
		return sat::HybridResult::MTV;
	} // SWEEP
	out_norm = tempEnter < enterTime ? -norm2 : norm1;
	out_t = enterTime;
	return sat::HybridResult::SWEEP;
}

bool sat::performSAT(const Shape* const first, const Shape* const second) {
	const std::vector<units::Coordinate2D> axes(_get_separating_axes(first, second));
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = first->getProjection(axes[i]);
		projSecond = second->getProjection(axes[i]);
		if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
			return false;
	}
	return true;
}

bool sat::performSAT(const Shape* const first, const units::Coordinate2D& firstPos, const Shape* const second, const units::Coordinate2D& secondPos) {
	const std::vector<units::Coordinate2D> axes(_get_separating_axes(first, second));
	const units::Coordinate2D offset(firstPos - secondPos);
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = first->getProjection(axes[i]);
		projSecond = second->getProjection(axes[i]);
		projFirst += offset.dot(axes[i]); // Apply offset between the two polygons' positions.
		if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
			return false;
	}
	return true;
}

bool sat::performSAT(const Polygon& first, const units::Coordinate2D& firstPos, const Polygon& second, const units::Coordinate2D& secondPos,
                     units::Coordinate2D& out_norm, units::Coordinate& out_dist) {
	units::Coordinate2D norm1(0, 0), norm2(0, 0);
	units::Coordinate dist1(-1), dist2(-1);
	if (!_MTV_SAT(first, second, firstPos - secondPos, norm1, dist1))
		return false;
	if (!_MTV_SAT(second, first, secondPos - firstPos, norm2, dist2))
		return false;
	if (dist1 < dist2) {
		out_norm = norm1;
		out_dist = dist1;
	} else {
		out_norm = -norm2; // norm2 is relative to second polygon: reverse it.
		out_dist = dist2;
	}
	return true;
}

sat::HybridResult sat::performHybridSAT(const Polygon& first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
	const Polygon& second, const units::Coordinate2D& secondPos, units::Coordinate2D& out_norm, units::Fraction& out_t) {
	if (firstDelta.isZero()) // No movement, just do regular SAT.
		return performSAT(first, firstPos, second, secondPos, out_norm, out_t) ? sat::HybridResult::MTV : sat::HybridResult::NONE;
	return _perform_hybrid_SAT(first, second, firstPos - secondPos, firstDelta, out_norm, out_t);
}

sat::HybridResult sat::performHybridSAT(const Polygon& first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
	const Polygon& second, const units::Coordinate2D& secondPos, const units::Coordinate2D& secondDelta, units::Coordinate2D& out_norm, units::Fraction& out_t) {
	const units::Coordinate2D delta = firstDelta - secondDelta; // Treat second as if it is stationary.
	return performHybridSAT(first, firstPos, delta, second, secondPos, out_norm, out_t);
}