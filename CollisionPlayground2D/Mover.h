#pragma once
#ifndef _MOVER_H
#define _MOVER_H

#include "Geometry/Movable.h"
#include "Geometry/ShapeContainer.h"
#include "Units.h"

class CollisionMap;

class Mover : public Movable {
public:
	static const units::Velocity     MAX_SPEED;
	static const units::Velocity     MAX_DIAGONAL_SPEED;
	static const units::Acceleration ACCELERATION;
	static const units::Acceleration DECELERATION;

	Mover() {}
	Mover(Movable::CollisionType type, const ShapeContainer& collider, const units::Coordinate2D& position);
	Mover(const ShapeContainer& collider, const units::Coordinate2D& position);
	~Mover() {}

	void update(const units::MS elapsedTime, const CollisionMap* const map);

	void setPosition(const units::Coordinate2D position);

	const units::Coordinate2D& getPosition() const;
	const ShapeContainer& getCollider() const;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingHorizontal();
	void stopMovingVertical();
	void stopMoving();
private:
	ShapeContainer collider_;
	units::Coordinate2D position_;

	units::Acceleration2D acceleration_;
	units::Velocity2D velocity_;

	void update_position(const units::MS elapsedTime, const units::Velocity maxSpeed, const CollisionMap* const polys);
	void _init() const;
};

#endif // _MOVER_H