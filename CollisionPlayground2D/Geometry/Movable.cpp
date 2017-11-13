#include "Movable.h"

#include <vector>
#include <iostream>

#include "DebugLogger.h"
#include "../Units.h"
#include "Shape.h"
#include "Polygon.h"
#include "SAT.h"
#include "CollisionMap.h"

const units::Coordinate Movable::COLLISION_BUFFER  = 0.001f;
const units::Coordinate Movable::WEDGE_MOVE_THRESH = 0.0001f;
const unsigned int      Movable::COLLISION_DEBUG_MAX_ATTEMPTS = 3;
const unsigned int      Movable::COLLISION_ALG_MAX_DEPTH = 25;

Movable::~Movable() {}

sat::HybridResult Movable::_find_closest_collision(const CollisionMap& collisionMap, Movable::CollisionInfo& info) const {
	units::Coordinate2D testNorm;
	units::Fraction interval(1.0f), testInterval;
	info.isCollision = false;
	const units::Coordinate2D delta(info.currentDir * info.remainingDist);
	std::vector<Polygon> objs = collisionMap.getColliding(*info.collider, info.currentPosition, delta);
	sat::HybridResult result;
	for (std::size_t i = 0; i < objs.size(); ++i) {
		result = sat::performHybridSAT(*info.collider, info.currentPosition, delta, objs[i], units::Coordinate2D(0, 0), testNorm, testInterval);
		if (result == sat::HybridResult::SWEEP) {
			info.isCollision = true;
			if (interval > testInterval) {
				interval = testInterval;
				info.normal = testNorm;
			}
			if (interval < constants::EPSILON) {
				info.moveDist = 0;
				return sat::HybridResult::SWEEP;
			}
		}
		if (result == sat::HybridResult::MTV) {
			info.isCollision = true;
			info.moveDist = testInterval;
			info.normal = testNorm;
			return sat::HybridResult::MTV; // Currently overlapping something. Abort.
		}
	}
	if (!info.isCollision) {
		info.moveDist = info.remainingDist;
		return sat::HybridResult::NONE;
	}
	info.moveDist = (info.remainingDist * interval) - getPushoutDistance(info.currentDir, info.normal);
	if (info.moveDist < 0)
		info.moveDist = 0;
	return sat::HybridResult::SWEEP;
}

units::Coordinate2D Movable::move(const units::Coordinate2D& origin, const Polygon& collider,
	                                 const units::Coordinate2D& delta, const CollisionMap& collisionMap) {
	const units::Coordinate originalDist = delta.magnitude();
	CollisionInfo info(&collider, origin, delta/originalDist, originalDist);
	if (delta.isZero())
		return origin; // Nowhere to move.
	switch (type) {
	case CollisionType::NONE:
		info.currentPosition += delta;
		break;
	case CollisionType::DEFLECTION:
		_move_deflection(info, collisionMap);
		break;
	case CollisionType::REVERSE:
		std::cerr << "Error: Unimplemented collision algorithm type.\n";
		break;
	case CollisionType::REFLECT:
		std::cerr << "Error: Unimplemented collision algorithm type.\n";
		break;
	case CollisionType::_DEBUG_:
		std::cerr << "Error: _DEBUG_ collision algorithm is for internal use only.\n";
		break;
	default:
		std::cerr << "Error: Unimplemented collision algorithm type.\n";
	}
	return info.currentPosition;
}

void Movable::_move_deflection(Movable::CollisionInfo& info, const CollisionMap& collisionMap) {
	unsigned int depth = 0;
	// To detect oscillating deflections where the mover isn't moving (is in a wedge), keep track of the
	// deflection angle relative to the original direction.
	// (This is the cosine of the angle: 0 == 90 degrees, an impossible deflection angle.)
	units::Coordinate prevAngle = 0;
	while (depth < COLLISION_ALG_MAX_DEPTH) {
		if (_find_closest_collision(collisionMap, info) == sat::HybridResult::MTV) {
			_debug_collision(info, collisionMap);
			return;
		}
		info.currentPosition += info.moveDist * info.currentDir;
		if (!info.isCollision)
			return;
		info.remainingDist -= info.moveDist;
		if (!onCollision(info))
			return; // Signaled to stop.
		if (info.remainingDist < constants::EPSILON || info.normal.isZero())
			return;
		// Find the projection of the remaining distance along the original direction on the deflection vector.
		// Get the vector to project along by rotating 90 degrees (direction doesn't matter).
		const units::Coordinate2D projDir(info.normal.perpCW());
		// Project using the original delta direction, to avoid "bouncing" off of corners.
		const units::Coordinate2D projection(info.originalDir.project(projDir, info.remainingDist));
		info.remainingDist = projection.magnitude(); // Projection is our new delta.
		if (info.remainingDist < constants::EPSILON)
			return;
		info.currentDir = projection / info.remainingDist;

		units::Coordinate currAngle = 0; // 0 == 90 degrees == an impossible angle of deflection/Movable has stopped.
		if (info.moveDist < WEDGE_MOVE_THRESH) {
			// Get signed angle of deflection relative to the original direction.
			const units::Coordinate dot(info.originalDir.dot(info.currentDir));
			currAngle = info.originalDir.cross(info.currentDir) < 0 ? -dot : dot;
			// If the previous angle is farther away from the original direction than the current angle, (and
			// we're still not moving), then we've begun to oscillate (we're getting more stuck, rather than "escaping").
			if (prevAngle != 0 && (prevAngle < 0 ? (prevAngle <= currAngle) : (prevAngle >= currAngle)))
				return;
		}
		prevAngle = currAngle;
		++depth;
#ifdef DEBUG
		if (depth >= 5)
			DBG_LOG("Recursion depth: " << depth << " moveDist: " << info.moveDist << " remainingDist: " << info.remainingDist);
#endif
	}
}

bool Movable::_debug_collision(CollisionInfo& info, const CollisionMap& collisionMap) {
	DBG_LOG("Debugging MTV collision...");
	info.currentPosition += (info.moveDist + COLLISION_BUFFER) * info.normal;
	for (std::size_t i = 1; i < COLLISION_DEBUG_MAX_ATTEMPTS; ++i) {
		std::vector<Polygon> objs = collisionMap.getColliding(*info.collider, info.currentPosition);
		info.isCollision = false;
		for (std::size_t k = 0; k < objs.size(); ++k) {
			if (sat::performSAT(*info.collider, info.currentPosition, objs[k], units::Coordinate2D(0, 0), info.normal, info.moveDist)) {
				info.isCollision = true;
				break;
			}
		}
		if (!info.isCollision) {
			DBG_LOG("MTV collision resolved (in " << i << " attempts).");
			return true; // Situation resolved. No longer overlapping anything.
		}
		info.currentPosition += (info.moveDist + COLLISION_BUFFER) * info.normal;;
	}
	DBG_WARN("Max debug attempts (" << COLLISION_DEBUG_MAX_ATTEMPTS << ") used. MTV collision may not be resolved.");
	return false; // May not be resolved.
}

bool Movable::onCollision(CollisionInfo & info){
	return true;
}