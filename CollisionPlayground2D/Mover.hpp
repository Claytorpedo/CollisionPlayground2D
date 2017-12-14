#pragma once
#ifndef INCLUDE_MOVER_HPP
#define INCLUDE_MOVER_HPP

#include "units.hpp"
#include "Geometry/Movable.hpp"
#include "Geometry/ShapeContainer.hpp"
#include "Geometry/units.hpp"

namespace geom { class CollisionMap; }

class Mover : public geom::Movable {
public:
	static const game::Velocity     MAX_SPEED;
	static const game::Velocity     MAX_DIAGONAL_SPEED;
	static const game::Acceleration ACCELERATION;
	static const game::Acceleration DECELERATION;

	Mover() {}
	Mover(geom::Movable::CollisionType type, const geom::ShapeContainer& collider, const geom::Coord2& position);
	Mover(const geom::ShapeContainer& collider, const geom::Coord2& position);
	~Mover() {}

	void update(const game::MS elapsedTime, const geom::CollisionMap& map);

	void setPosition(const geom::Coord2& position);

	const geom::Coord2& getPosition() const;
	const geom::ShapeContainer& getCollider() const;

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingHorizontal();
	void stopMovingVertical();
	void stopMoving();
private:
	geom::ShapeContainer collider_;
	geom::Coord2 position_;

	game::Acceleration2D acceleration_;
	game::Velocity2D velocity_;

	void update_position(const game::MS elapsedTime, const game::Velocity maxSpeed, const geom::CollisionMap& map);
	void _init() const;
};

#endif // INCLUDE_MOVER_HPP
