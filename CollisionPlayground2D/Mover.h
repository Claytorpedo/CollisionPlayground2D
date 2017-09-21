#pragma once
#ifndef _MOVER_H
#define _MOVER_H

#include "Geometry/Collidable.h"
#include "Geometry/Polygon.h"
#include "Units.h"

class CollisionMap;

namespace mover {
	const units::Velocity     MAX_SPEED =          0.3f;
	const units::Velocity     MAX_DIAGONAL_SPEED = MAX_SPEED * (units::Velocity)std::sin(constants::PI / 4.0f);
	const units::Acceleration ACCELERATION       = 0.0025f;
	const units::Acceleration DECELERATION       = 0.004f;
}

class Mover : public Collidable {
public:
	Mover() {}
	Mover(Collidable::CollisionType type, units::Coordinate2D position, Polygon collider);
	Mover(units::Coordinate2D position, Polygon collider);
	~Mover() {}

	void update(const units::MS elapsedTime, const CollisionMap& polys);

	void setPosition(const units::Coordinate2D position);

	Polygon getCollider() const;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingHorizontal();
	void stopMovingVertical();
	void stopMoving();
private:
	units::Coordinate2D position_;
	Polygon collider_;

	units::Acceleration2D acceleration_;
	units::Velocity2D velocity_;

	void update_position(const units::MS elapsedTime, const units::Velocity maxSpeed, const CollisionMap& polys);
};

#endif // _MOVER_H