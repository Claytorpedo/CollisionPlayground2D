#include "Collidable.h"

#include <vector>
#include <iostream>

#include "../Units.h"
#include "Shape.h"
#include "Polygon.h"
#include "CollisionMath.h"

Collidable::~Collidable() {}

void Collidable::find_closest_collision(const Polygon& collider, const std::vector<Polygon>& polys, Collidable::CollisionInfo& info) const {
	const Polygon clippedCollider = collider.clipExtend(info.currentDir, info.remainingDist);
	info.moveDist = info.remainingDist;
	for (std::size_t i = 0; i < polys.size(); ++i) {
		units::Coordinate testDist;
		units::Coordinate2D testNorm;
		if (collision_math::clippedCollides(clippedCollider, info.currentDir, info.remainingDist, polys[i], testDist, testNorm)) {
			info.isCollision = true;
			if (info.moveDist > testDist) {
				info.moveDist = testDist;
				info.normal = testNorm;
			}
			if (info.moveDist == 0.0f)
				return; // Can't move any less than not moving!
		}
	}
}

units::Coordinate2D Collidable::move(const units::Coordinate2D& origin, const Polygon& collider,
	                                 const units::Coordinate2D& delta, const std::vector<Polygon>& polys) {
	const units::Coordinate originalDist = delta.magnitude();
	CollisionInfo info(&collider, origin, delta/originalDist, originalDist);
	if (delta.isZero())
		return origin; // Nowhere to move.
	switch (type) {
	case DEFLECTION:
		move_deflection(info, polys);
		break;
	case REVERSE:

	case REFLECT:

	default:
		std::cerr << "Error: Unimplemented collision algorithm type.\n";
	}
	return info.currentPosition;
}

void Collidable::move_deflection(Collidable::CollisionInfo& info, const std::vector<Polygon>& polys) {
	Polygon collider(*info.collider);
	collider.translate(info.currentPosition);
#ifdef DEBUG
	int depth = 0;
#endif
	// To detect oscillating deflections where the mover isn't moving (is in a wedge), keep track of the
	// deflection angle relative to the original direction.
	// (This is the cosine of the angle: 0 == 90 degrees, an impossible deflection angle.)
	units::Coordinate prevAngle = 0;
	while (true) {
		find_closest_collision(collider, polys, info);
		info.currentPosition += info.currentDir*info.moveDist;
		if (!info.isCollision)
			return;
		if (!onCollision(info))
			return; // Signaled to stop.
		collider = Polygon::translate(*info.collider, info.currentPosition);
		info.remainingDist -= info.moveDist;
		if (info.remainingDist < constants::EPSILON || info.normal.isZero())
			return;
		// Find the projection of the remaining distance along the original direction on the deflection vector.
		// Get the edge vector to project along by rotating clockwise.
		const units::Coordinate2D projDir = info.normal.perpCW().normalize();
		// Project using the original delta direction, to avoid "bouncing" off of corners.
		const units::Coordinate2D projection(info.originalDir.project(projDir, info.remainingDist));
		// Projection is our new delta. Get new direction and remaining distance to move.
		info.remainingDist = projection.magnitude();
		if (info.remainingDist < constants::EPSILON)
			return;
		info.currentDir = projection / info.remainingDist;

		units::Coordinate currAngle = 0; // 0 == 90 degrees == an impossible angle of deflection/collidable has stopped.
		if (info.moveDist == 0) {
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
			std::cout << "Recursion depth: " << depth << std::endl;
#endif
	}
}

bool Collidable::onCollision(CollisionInfo & info){
	return true;
}