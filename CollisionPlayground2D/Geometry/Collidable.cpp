#include "Collidable.h"

#include <vector>
#include <iostream>

#include "../Units.h"
#include "Shape.h"
#include "Polygon.h"
#include "SAT.h"
#include "CollisionMap.h"

Collidable::~Collidable() {}

// Keep a small space buffer around a polygon when moving towards it, to avoid moving into a currently-colliding state.
// Acts as if making the polygon slightly larger.
const units::Coordinate COLLISION_BUFFER = 0.001f;
void Collidable::find_closest_collision(const Polygon& collider, const CollisionMap& collisionMap, Collidable::CollisionInfo& info) const {
	units::Coordinate2D testNorm;
	units::Fraction interval(1.0f), testInterval;
	info.isCollision = false;
	const units::Coordinate2D delta(info.currentDir * info.remainingDist);
	std::vector<Polygon> objs = collisionMap.getColliding(collider, delta);
	for (std::size_t i = 0; i < objs.size(); ++i) {
		if (sat::performHybridSAT(collider, info.currentPosition, delta, objs[i], units::Coordinate2D(0, 0), testNorm, testInterval)) {
			info.isCollision = true;
			if (testInterval < 0.0f) {
				std::cerr << "Error: got an MTV collision! (dist = " << testInterval << ")\n";
			}
			if (interval > testInterval) {
				interval = testInterval;
				info.normal = testNorm;
			}
			if (interval < constants::EPSILON) {
				info.moveDist = 0;
				return;
			}
		}
	}
	if (!info.isCollision) {
		info.moveDist = info.remainingDist;
		return;
	}
	// Get the buffer amount to maintain to avoid moving to a collision state.
	// buffer_dist / cos(theta) = hypotenuse; cos(theta) = norm * dir (norm should be reversed, but we can just negate the end product).
	const units::Coordinate pushout = COLLISION_BUFFER / info.normal.dot(info.currentDir);
	info.moveDist = (info.remainingDist * interval) + pushout; // Add instead of subtract to negate.awd
	if (info.moveDist < 0)
		info.moveDist = 0;
}

units::Coordinate2D Collidable::move(const units::Coordinate2D& origin, const Polygon& collider,
	                                 const units::Coordinate2D& delta, const CollisionMap& collisionMap) {
	const units::Coordinate originalDist = delta.magnitude();
	CollisionInfo info(&collider, origin, delta/originalDist, originalDist);
	if (delta.isZero())
		return origin; // Nowhere to move.
	switch (type) {
	case DEFLECTION:
		move_deflection(info, collisionMap);
		break;
	case REVERSE:

	case REFLECT:

	default:
		std::cerr << "Error: Unimplemented collision algorithm type.\n";
	}
	return info.currentPosition;
}

void Collidable::move_deflection(Collidable::CollisionInfo& info, const CollisionMap& collisionMap) {
#ifdef DEBUG
	int depth = 0;
#endif
	// To detect oscillating deflections where the mover isn't moving (is in a wedge), keep track of the
	// deflection angle relative to the original direction.
	// (This is the cosine of the angle: 0 == 90 degrees, an impossible deflection angle.)
	units::Coordinate prevAngle = 0;
	while (true) {
		find_closest_collision(*info.collider, collisionMap, info);
		info.currentPosition += info.moveDist * info.currentDir;
		if (!info.isCollision)
			return;
		info.remainingDist -= info.moveDist;
		if (!onCollision(info))
			return; // Signaled to stop.
		if (info.remainingDist < constants::EPSILON || info.normal.isZero())
			return;
		// Find the projection of the remaining distance along the original direction on the deflection vector.
		// Get the vector to project along by rotating clockwise.
		const units::Coordinate2D projDir(info.normal.perpCW());
		// Project using the original delta direction, to avoid "bouncing" off of corners.
		const units::Coordinate2D projection(info.originalDir.project(projDir, info.remainingDist));
		info.remainingDist = projection.magnitude(); // Projection is our new delta.
		if (info.remainingDist < constants::EPSILON)
			return;
		info.currentDir = projection / info.remainingDist;

		units::Coordinate currAngle = 0; // 0 == 90 degrees == an impossible angle of deflection/collidable has stopped.
		if (info.moveDist < constants::EPSILON * 10) {
			// Get signed angle of deflection relative to the original direction.
			const units::Coordinate dot(info.originalDir.dot(info.currentDir));
			currAngle = info.originalDir.cross(info.currentDir) < 0 ? -dot : dot;
			// If the previous angle is farther away from the original direction than the current angle, (and
			// we're still not moving), then we've begun to oscillate (we're getting more stuck, rather than "escaping").
			if (prevAngle != 0 && (prevAngle < 0 ? (prevAngle <= currAngle) : (prevAngle >= currAngle)))
				return;
		}
		prevAngle = currAngle;
#ifdef DEBUG
		++depth;
		if (depth >= 10)
			std::cout << "Recursion depth: " << depth << " movedist: " << info.moveDist << std::endl;
#endif
	}
}

bool Collidable::onCollision(CollisionInfo & info){
	return true;
}