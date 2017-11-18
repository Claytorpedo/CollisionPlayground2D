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
inline std::vector<units::Coordinate2D> sat::getSeparatingAxes(const Shape* const first, const Shape* const second) {
	std::vector<units::Coordinate2D> axes;
	if (_get_separating_axes(first, second, axes, true))
		return axes;
	_get_separating_axes(second, first, axes, false);
	return axes;
}

const static units::Fraction MAX_TIME = 1.0f; // Using interval [0,1].
// Tests the axes of one polygon against the other using SAT. Checks if they are currently overlapping, or will overlap in the future (SAT test and sweep test).
// Note that out_enterTime, out_exitTime, and out_mtv_dist need to be set to defaults on the first call.
// offset      - the position of first - second.
// relativeVel - the velocity of first - second (we act as if only first is moving).
// out_norm    - the normal of collision, or direction of separation for the case where the shapes are already overlapping.
// out_t       - the time of collision, or the distance to move for the case where the shapes are already overlapping.
// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
inline sat::HybridResult _perform_hybrid_SAT(const Shape* const first, const Shape* const second, const units::Coordinate2D& offset,
                                             const units::Coordinate2D& relativeVel, units::Coordinate2D& out_norm, units::Fraction& out_t) {
	const std::vector<units::Coordinate2D> axes = sat::getSeparatingAxes(first, second);
	bool areCurrentlyOverlapping = true; // Start by assuming they are overlapping.
	units::Coordinate mtv_dist(-1), testDist, overlap1, overlap2;
	units::Fraction speed, enterTime(-1), exitTime(MAX_TIME), testEnter, testExit;
	units::Coordinate2D mtv_norm, sweep_norm;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = first->getProjection(axes[i]);
		projSecond = second->getProjection(axes[i]);
		projFirst += offset.dot(axes[i]); // Apply offset between the two polygons' positions.
		overlap1 = projFirst.max - projSecond.min - constants::EPSILON;
		overlap2 = projSecond.max - projFirst.min - constants::EPSILON;
		speed = relativeVel.dot(axes[i]); // Speed projected along this axis.
		if (overlap1 < 0.0f || overlap2 < 0.0f) { // Not currently overlapping.
			areCurrentlyOverlapping = false;
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
			if (testEnter > enterTime) {
				enterTime = testEnter; // We want the latest time: the first time when all axes overlap.
				// The last axis to overlap will have the collision normal.
				sweep_norm = projFirst.min < projSecond.min ? -axes[i] : axes[i]; // Collision normal is relative to the first shape.
			}
			if (testExit < exitTime)
				exitTime = testExit; // Keep track of earliest exit time: some axis may stop overlapping before all axes overlap.
			if (enterTime > MAX_TIME || enterTime > exitTime)
				return sat::HybridResult::NONE; // Either don't collide on this time interval, or won't ever with the direction of motion.
		} else { // They are currently overlapping on this axis.
			if (speed != 0) { // Find when the time when they stop overlapping on this axis (start time == 0 == now).
				testExit = (speed < 0 ? (-overlap1) : overlap2) / speed;
				if (testExit < exitTime)
					exitTime = testExit;
				if (enterTime > exitTime) // There is no interval where all axes have overlap.
					return sat::HybridResult::NONE;
			}
			if (areCurrentlyOverlapping) { // Regular MTV checks.
				testDist = (projFirst.min < projSecond.min ? overlap1 : overlap2) + constants::EPSILON; // Find separation for this axis.
				if (mtv_dist == -1 || testDist < mtv_dist) {
					mtv_dist = testDist;
					mtv_norm = projFirst.min < projSecond.min ? -axes[i] : axes[i]; // Pushout direction for the first shape.
				}
			}
		}
	}
	if (areCurrentlyOverlapping) {
		out_norm = mtv_norm;
		out_t = mtv_dist;
		return sat::HybridResult::MTV;
	}
	out_norm = sweep_norm;
	out_t = enterTime;
	return sat::HybridResult::SWEEP;
}

bool sat::performSAT(const Shape* const first, const Shape* const second) {
	const std::vector<units::Coordinate2D> axes(getSeparatingAxes(first, second));
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
	const std::vector<units::Coordinate2D> axes(getSeparatingAxes(first, second));
	const units::Coordinate2D offset(firstPos - secondPos);
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = first->getProjection(axes[i]);
		projSecond = second->getProjection(axes[i]);
		projFirst += offset.dot(axes[i]); // Apply offset between the two shapes' positions.
		if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
			return false;
	}
	return true;
}

bool sat::performSAT(const Shape* const first, const units::Coordinate2D& firstPos, const Shape* const second, const units::Coordinate2D& secondPos,
                     units::Coordinate2D& out_norm, units::Coordinate& out_dist) {
	const std::vector<units::Coordinate2D> axes(getSeparatingAxes(first, second));
	const units::Coordinate2D offset(firstPos - secondPos);
	units::Coordinate2D norm, testNorm;
	units::Coordinate overlap1, overlap2, minDist(-1), testDist;
	Projection projFirst, projSecond;
	for (std::size_t i = 0; i < axes.size(); ++i) {
		projFirst = first->getProjection(axes[i]);
		projSecond = second->getProjection(axes[i]);
		projFirst += offset.dot(axes[i]); // Apply offset between the two shapes' positions.
		overlap1 = projFirst.max - projSecond.min;
		overlap2 = projSecond.max - projFirst.min;
		if (overlap1 < constants::EPSILON || overlap2 < constants::EPSILON)
			return false;
		// Find separation for this axis.
		if (projFirst.min < projSecond.min) {
			testDist = overlap1;
			testNorm = -axes[i]; // Ensure right direction to pushout the first shape.
		} else {
			testDist = overlap2;
			testNorm = axes[i];
		}
		if (minDist == -1 || testDist < minDist) {
			minDist = testDist;
			norm = testNorm;
		}
	}
	out_norm = norm;
	out_dist = minDist;
	return true;
}

sat::HybridResult sat::performHybridSAT(const Shape* const first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
	const Shape* const second, const units::Coordinate2D& secondPos, units::Coordinate2D& out_norm, units::Fraction& out_t) {
	if (firstDelta.isZero()) // No movement, just do regular SAT.
		return performSAT(first, firstPos, second, secondPos, out_norm, out_t) ? sat::HybridResult::MTV : sat::HybridResult::NONE;
	return _perform_hybrid_SAT(first, second, firstPos - secondPos, firstDelta, out_norm, out_t);
}

sat::HybridResult sat::performHybridSAT(const Shape* const first, const units::Coordinate2D& firstPos, const units::Coordinate2D& firstDelta,
	const Shape* const second, const units::Coordinate2D& secondPos, const units::Coordinate2D& secondDelta, units::Coordinate2D& out_norm, units::Fraction& out_t) {
	const units::Coordinate2D delta = firstDelta - secondDelta; // Treat second as if it is stationary.
	return performHybridSAT(first, firstPos, delta, second, secondPos, out_norm, out_t);
}