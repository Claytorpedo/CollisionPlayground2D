#pragma once
#ifndef INCLUDE_GAME_MOVER_HPP
#define INCLUDE_GAME_MOVER_HPP

#include "../units.hpp"
#include "../Input.hpp"

#include <Geometry2D/Geometry.hpp>

namespace geom { class CollisionMap; }
class Input;

namespace game {
	class Mover : public geom::Movable {
	public:
		static const game::Velocity     MAX_SPEED;
		static const game::Velocity     MAX_DIAGONAL_SPEED;
		static const game::Acceleration ACCELERATION;
		static const game::Acceleration DECELERATION;

		Mover() = default;
		Mover(geom::Movable::CollisionType type, const geom::ShapeContainer& collider, const geom::Coord2& position);
		Mover(const geom::ShapeContainer& collider, const geom::Coord2& position);

		void update(const game::MS elapsedTime, const geom::CollisionMap& map);

		void setPosition(const geom::Coord2& position);

		geom::Coord2 getPosition() const;
		geom::ConstShapeRef getCollider() const;

		void receiveInput(const Input& input);

		void moveLeft();
		void moveRight();
		void moveUp();
		void moveDown();
		void stopMovingHorizontal();
		void stopMovingVertical();
		void stopMoving();
	protected:
		virtual bool onCollision(geom::Movable::CollisionInfo& info);
	private:
		geom::ShapeContainer collider_{ geom::Rect{} };
		geom::Coord2 position_;

		game::Acceleration2D acceleration_;
		game::Velocity2D velocity_;

		void _init();
		void _update_position(const game::MS elapsedTime, const game::Velocity maxSpeed, const geom::CollisionMap& map);
	};
}

#endif // INCLUDE_GAME_MOVER_HPP
