#ifndef INCLUDE_GAME_MOVER_HPP
#define INCLUDE_GAME_MOVER_HPP

#include "../units.hpp"
#include "../Input.hpp"

#include <Geometry2D/Geometry.hpp>

namespace geom { class CollisionMap; }
class Input;

namespace game {
class Mover : public ctp::Movable {
public:
	static const game::Velocity     MAX_SPEED;
	static const game::Velocity     MAX_DIAGONAL_SPEED;
	static const game::Acceleration ACCELERATION;
	static const game::Acceleration DECELERATION;

	Mover() = default;
	Mover(ctp::Movable::CollisionType type, const ctp::ShapeContainer& collider, const ctp::Coord2& position);
	Mover(const ctp::ShapeContainer& collider, const ctp::Coord2& position);

	void update(const game::MS elapsedTime, const ctp::CollisionMap& map);

	void setPosition(const ctp::Coord2& position);

	ctp::Coord2 getPosition() const;
	ctp::ConstShapeRef getCollider() const;

	void receiveInput(const Input& input);

	void moveLeft();
	void moveRight();
	void moveUp();
	void moveDown();
	void stopMovingHorizontal();
	void stopMovingVertical();
	void stopMoving();
protected:
	virtual bool onCollision(ctp::Movable::CollisionInfo& info);
private:
	ctp::ShapeContainer collider_{ctp::Rect{}};
	ctp::Coord2 position_;

	game::Acceleration2D acceleration_;
	game::Velocity2D velocity_;

	void _init();
	void _update_position(const game::MS elapsedTime, const game::Velocity maxSpeed, const ctp::CollisionMap& map);
};
}

#endif // INCLUDE_GAME_MOVER_HPP
