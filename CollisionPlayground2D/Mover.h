#pragma once
#ifndef _MOVER_H
#define _MOVER_H

#include "Geometry/Movable.h"
#include "Geometry/Polygon.h"
#include "Units.h"

class CollisionMap;

class Mover : public Movable {
public:
	static const units::Velocity     MAX_SPEED;
	static const units::Velocity     MAX_DIAGONAL_SPEED;
	static const units::Acceleration ACCELERATION;
	static const units::Acceleration DECELERATION;

	Mover() {}
	Mover(Movable::CollisionType type, units::Coordinate2D position, Polygon collider);
	Mover(units::Coordinate2D position, Polygon collider);
	~Mover() {}

	void update(const units::MS elapsedTime, const CollisionMap& polys);

	void setPosition(const units::Coordinate2D position);

	const units::Coordinate2D& getPosition() const;
	const Shape* const getCollider() const;

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