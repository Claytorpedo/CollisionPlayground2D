#include "SAT.h"

#include <vector>

#include "Units.h"
#include "Constants.h"
#include "geom_math.h"
#include "DebugLogger.h"
#include "Shape.h"
#include "Polygon.h"
#include "Rectangle.h"
#include "Circle.h"
#include "Projection.h"
#include "ShapeContainer.h"

namespace geom {
	namespace sat {
		/*
		System for finding the separating axes for the given shapes.
		Determine the type of shape the first one is, then see if it forms a special case when paired with the second shape.
		Returns true if it encounteres a special case that handled both shapes.
		*/
		inline bool _get_separating_axes(const ShapeContainer& first, const ShapeContainer& second, const Coord2& offset, std::vector<Coord2>& axes) {
			switch (first.type()) {
			case (ShapeType::RECTANGLE):
				axes.push_back(Coord2(1, 0)); // Rectangles are axis-alligned.
				axes.push_back(Coord2(0, 1));
				if (second.type() == ShapeType::RECTANGLE) // Rectangles will share axes.
					return true;
				break;
			case (ShapeType::POLYGON):
				axes.reserve(axes.size() + first.poly().size());
				for (std::size_t i = 0; i < first.poly().size(); ++i)
					axes.push_back(first.poly().getEdgeNorm(i));
				break;
			case (ShapeType::CIRCLE):
			{
				const Coord2 firstPos(first.circle().center + offset);
				if (second.type() == ShapeType::CIRCLE) { // Only one axis for two circles.
					const Coord2 axis = firstPos - second.circle().center;
					axes.push_back(axis.x == 0 && axis.y == 0 ? Coord2(0, 1) : axis.normalize());
					return true;
				}
				// Get axis from circle to the cloeset point/vertex on the other shape.
				const Coord2 axis = second.shape().getClosestTo(firstPos) - firstPos;
				if (axis.x != 0 || axis.y != 0) // If this is a zero vector, they are already overlapping, and can be separated with other axes.
					axes.push_back(axis.normalize());
			}
			break;
			default:
				DBG_WARN("Unhandled shape type for SAT. Converting to polygon.");
				Polygon p = first.shape().toPoly();
				for (std::size_t i = 0; i < p.size(); ++i)
					axes.push_back(p.getEdgeNorm(i));
			}
			return false;
		}
		// Gets the separating axes for two shapes.
		inline std::vector<Coord2> getSeparatingAxes(const ShapeContainer& first, const ShapeContainer& second, const Coord2& offset) {
			std::vector<Coord2> axes;
			if (_get_separating_axes(first, second, offset, axes))
				return axes;
			_get_separating_axes(second, first, -offset, axes);
			return axes;
		}

		inline HybridResult _circle_circle_hybrid_SAT(const Circle& first, const Circle& second, const Coord2& offset,
			const Coord2& relativeVel, Coord2& out_norm, gFloat& out_t) {
			const Coord2 firstPos(first.center + offset);
			const Coord2 separation(firstPos - second.center);
			const gFloat dist2(separation.magnitude2());
			const gFloat fullRad(first.radius + second.radius - constants::EPSILON); // Radius of both minus eps, for comparison.
			const gFloat fullRad2(fullRad * fullRad);
			if (dist2 <= fullRad2) { // They are currently overlapping.
				const gFloat dist(std::sqrt(dist2));
				out_t = first.radius + second.radius - dist;
				out_norm = separation / dist; // Normalize.
				return HybridResult::MTV;
			}
			const Coord2 closestTo(closestPointOnLine(firstPos, firstPos + relativeVel, second.center));
			const gFloat closestDist2((second.center - closestTo).magnitude2());
			if (closestDist2 >= fullRad2)
				return HybridResult::NONE; // They are not on a collision course.
			// They will collide some time in the future.
			const gFloat distFromClosestToCollision(std::sqrt(fullRad2 - closestDist2)); // Solve triangle.
			const gFloat deltaLen2(relativeVel.magnitude2());
			const gFloat deltaLen(std::sqrt(deltaLen2));
			const Coord2 deltaDir(relativeVel / deltaLen);
			// Determine the point of collision.
			const Coord2 collisionPoint(closestTo - distFromClosestToCollision * deltaDir);
			const gFloat distFromFirst2((collisionPoint - firstPos).magnitude2());
			if (distFromFirst2 > deltaLen2)
				return HybridResult::NONE; // It collides too far in the future.
			out_t = std::sqrt(distFromFirst2) / deltaLen;
			out_norm = (collisionPoint - second.center).normalize();
			return HybridResult::SWEEP;
		}
		inline HybridResult _circle_poly_hybrid_SAT(const Circle& first, const Polygon& second, const Coord2& offset,
			const Coord2& relativeVel, Coord2& out_norm, gFloat& out_t) {
			// Get axes on other shape and do hybrid sat, logging best t and norm.

			// Get axes from circle to poly to finish up MTV test. Don't log t or norm for this.

			// Get vertices on poly on opposide side of delta dir.

			// Do tests with vertices, similar to circle circle, with attention paid to early out cases.
			// >> find points with ray tests, and then do inside tests and whatever.
			// >> probably still need closest dist to line test to detect the pass-by case
			return HybridResult::NONE;
		}

		// Handle special cases for hybrid SAT.
		inline bool _handle_hybrid_sat_special_cases(const ShapeContainer& first, const ShapeContainer& second, const Coord2& offset,
			const Coord2& relativeVel, Coord2& out_norm, gFloat& out_t, HybridResult& r) {
			if (first.type() == ShapeType::CIRCLE) {
				if (second.type() == ShapeType::CIRCLE)
					r = _circle_circle_hybrid_SAT(first.circle(), second.circle(), offset, relativeVel, out_norm, out_t);
				else
					r = _circle_poly_hybrid_SAT(first.circle(), second.shape().toPoly(), offset, relativeVel, out_norm, out_t);
				return true;
			} else if (second.type() == ShapeType::CIRCLE) {
				r = _circle_poly_hybrid_SAT(second.circle(), first.shape().toPoly(), -offset, -relativeVel, out_norm, out_t);
				out_norm = -out_norm; // Give normal relative to first shape.
				return true;;
			}
			return false;
		}

		const static gFloat MAX_TIME = 1.0f; // Using interval [0,1].
		// Tests the axes of one polygon against the other using SAT. Checks if they are currently overlapping, or will overlap in the future (SAT test and sweep test).
		// Note that out_enterTime, out_exitTime, and out_mtv_dist need to be set to defaults on the first call.
		// axes        - the separating axes for these shapes.
		// offset      - the position of first - second.
		// relativeVel - the velocity of first - second (we act as if only first is moving).
		// out_norm    - the normal of collision, or direction of separation for the case where the shapes are already overlapping.
		// out_t       - the time of collision, or the distance to move for the case where the shapes are already overlapping.
		// Returns the type of collision: NONE, a current MTV collision, or a future SWEEP collision on the interval [0, MAX].
		inline HybridResult _perform_hybrid_SAT(const Shape& first, const Shape& second, const std::vector<Coord2>& axes, const Coord2& offset,
			const Coord2& relativeVel, Coord2& out_norm, gFloat& out_t) {
			bool areCurrentlyOverlapping = true; // Start by assuming they are overlapping.
			gFloat mtv_dist(-1), testDist, overlap1, overlap2;
			gFloat speed, enterTime(-1), exitTime(MAX_TIME), testEnter, testExit;
			Coord2 mtv_norm, sweep_norm;
			Projection projFirst, projSecond;
			for (std::size_t i = 0; i < axes.size(); ++i) {
				projFirst = first.getProjection(axes[i]);
				projSecond = second.getProjection(axes[i]);
				projFirst += offset.dot(axes[i]); // Apply offset between the two polygons' positions.
				overlap1 = projFirst.max - projSecond.min - constants::EPSILON;
				overlap2 = projSecond.max - projFirst.min - constants::EPSILON;
				speed = relativeVel.dot(axes[i]); // Speed projected along this axis.
				if (overlap1 < 0.0f || overlap2 < 0.0f) { // Not currently overlapping.
					areCurrentlyOverlapping = false;
					if (speed == 0)
						return HybridResult::NONE; // Not moving on this axis (moving parallel, or not at all). They will never meet.
					// Overlaps now tell us how far apart they are on this axis. Divide by speed on this axis to find if/when they will overlap.
					if (overlap1 < 0.0f) { // The projection of the first shape is to the "left" of the second on this axis.
						testEnter = (-overlap1) / speed;
						testExit = overlap2 / speed;
					} else { // Interval on overlap2's side: projection of the first shape is to the "right" of the second on this axis.
						testEnter = overlap2 / speed;
						testExit = (-overlap1) / speed;
					}
					if (testEnter < 0.0f)
						return HybridResult::NONE; // They are moving apart on this axis.
					if (testEnter > enterTime) {
						enterTime = testEnter; // We want the latest time: the first time when all axes overlap.
						// The last axis to overlap will have the collision normal.
						sweep_norm = projFirst.min < projSecond.min ? -axes[i] : axes[i]; // Collision normal is relative to the first shape.
					}
					if (testExit < exitTime)
						exitTime = testExit; // Keep track of earliest exit time: some axis may stop overlapping before all axes overlap.
					if (enterTime > MAX_TIME || enterTime > exitTime)
						return HybridResult::NONE; // Either don't collide on this time interval, or won't ever with the direction of motion.
				} else { // They are currently overlapping on this axis.
					if (speed != 0) { // Find when the time when they stop overlapping on this axis (start time == 0 == now).
						testExit = (speed < 0 ? (-overlap1) : overlap2) / speed;
						if (testExit < exitTime)
							exitTime = testExit;
						if (enterTime > exitTime) // There is no interval where all axes have overlap.
							return HybridResult::NONE;
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
				return HybridResult::MTV;
			}
			out_norm = sweep_norm;
			out_t = enterTime;
			return HybridResult::SWEEP;
		}

		bool performSAT(const ShapeContainer& first, const ShapeContainer& second) {
			const std::vector<Coord2> axes(getSeparatingAxes(first, second));
			const Shape &firstShape(first.shape()), &secondShape(second.shape());
			Projection projFirst, projSecond;
			for (std::size_t i = 0; i < axes.size(); ++i) {
				projFirst = firstShape.getProjection(axes[i]);
				projSecond = secondShape.getProjection(axes[i]);
				if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
					return false;
			}
			return true;
		}

		bool performSAT(const ShapeContainer& first, const Coord2& firstPos, const ShapeContainer& second, const Coord2& secondPos) {
			const Coord2 offset(firstPos - secondPos);
			const std::vector<Coord2> axes(getSeparatingAxes(first, second, offset));
			const Shape &firstShape(first.shape()), &secondShape(second.shape());
			Projection projFirst, projSecond;
			for (std::size_t i = 0; i < axes.size(); ++i) {
				projFirst = firstShape.getProjection(axes[i]);
				projSecond = secondShape.getProjection(axes[i]);
				projFirst += offset.dot(axes[i]); // Apply offset between the two shapes' positions.
				if (projFirst.min + constants::EPSILON > projSecond.max || projFirst.max < projSecond.min + constants::EPSILON)
					return false;
			}
			return true;
		}

		bool performSAT(const ShapeContainer& first, const Coord2& firstPos, const ShapeContainer& second, const Coord2& secondPos,
			Coord2& out_norm, gFloat& out_dist) {
			const Coord2 offset(firstPos - secondPos);
			const std::vector<Coord2> axes(getSeparatingAxes(first, second, offset));
			const Shape &firstShape(first.shape()), &secondShape(second.shape());
			Coord2 norm, testNorm;
			gFloat overlap1, overlap2, minDist(-1), testDist;
			Projection projFirst, projSecond;
			for (std::size_t i = 0; i < axes.size(); ++i) {
				projFirst = firstShape.getProjection(axes[i]);
				projSecond = secondShape.getProjection(axes[i]);
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

		HybridResult performHybridSAT(const ShapeContainer& first, const Coord2& firstPos, const Coord2& firstDelta,
			const ShapeContainer& second, const Coord2& secondPos, Coord2& out_norm, gFloat& out_t) {
			if (firstDelta.isZero()) // No movement, just do regular SAT.
				return performSAT(first, firstPos, second, secondPos, out_norm, out_t) ? HybridResult::MTV : HybridResult::NONE;
			const Coord2 offset(firstPos - secondPos);
			HybridResult r;
			if (_handle_hybrid_sat_special_cases(first, second, offset, firstDelta, out_norm, out_t, r))
				return r;
			return _perform_hybrid_SAT(first.shape(), second.shape(), getSeparatingAxes(first, second, offset), offset, firstDelta, out_norm, out_t);
		}

		HybridResult performHybridSAT(const ShapeContainer& first, const Coord2& firstPos, const Coord2& firstDelta,
			const ShapeContainer& second, const Coord2& secondPos, const Coord2& secondDelta, Coord2& out_norm, gFloat& out_t) {
			const Coord2 delta = firstDelta - secondDelta; // Treat second as if it is stationary.
			return performHybridSAT(first, firstPos, delta, second, secondPos, out_norm, out_t);
		}
	}
}