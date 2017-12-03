#include "Mover.h"

#include "Geometry/ShapeContainer.h"
#include "Geometry/Polygon.h"
#include "Geometry/CollisionMap.h"
#include "Geometry/Util.h"

const game::Velocity     Mover::MAX_SPEED = 0.3f;
const game::Velocity     Mover::MAX_DIAGONAL_SPEED = Mover::MAX_SPEED * (game::Velocity)std::sin(geom::constants::PI / 4.0f);
const game::Acceleration Mover::ACCELERATION = 0.0025f;
const game::Acceleration Mover::DECELERATION = 0.004f;

void Mover::_init() const {
	if (collider_.type() == geom::ShapeType::POLYGON)
		collider_.poly().computeNormals();
}

Mover::Mover(geom::Movable::CollisionType type, const geom::ShapeContainer& collider, const geom::Coord2& position) : Movable(type), collider_(collider), position_(position) {
	_init();
}
Mover::Mover(const geom::ShapeContainer& collider, const geom::Coord2& position) : collider_(collider), position_(position) {
	_init();
}

void Mover::update(const game::MS elapsedTime, const geom::CollisionMap* const map) {
	const game::Velocity maxSpeed = (!geom::util::almostZero(acceleration_.x) && !geom::util::almostZero(acceleration_.y)) ? MAX_DIAGONAL_SPEED : MAX_SPEED;
	update_position(elapsedTime, maxSpeed, map);
}

void Mover::update_position(const game::MS elapsedTime, const game::Velocity maxSpeed, const geom::CollisionMap* const map) {
	velocity_ += acceleration_ * (geom::gFloat)(elapsedTime);
	velocity_.x = geom::util::clamp(velocity_.x, -maxSpeed, maxSpeed);
	velocity_.y = geom::util::clamp(velocity_.y, -maxSpeed, maxSpeed);
	if (acceleration_.x == 0 && velocity_.x != 0) {
		const bool isPos(velocity_.x > 0);
		velocity_.x += (isPos ? -1.0f : 1.0f) * DECELERATION * elapsedTime;
		velocity_.x = isPos ? (velocity_.x < 0 ? 0.0f : velocity_.x) : (velocity_.x > 0 ? 0.0f : velocity_.x);
	}
	if (acceleration_.y == 0 && velocity_.y != 0) {
		const bool isPos(velocity_.y > 0);
		velocity_.y += (isPos ? -1.0f : 1.0f) * DECELERATION * elapsedTime;
		velocity_.y = isPos ? (velocity_.y < 0 ? 0.0f : velocity_.y) : (velocity_.y > 0 ? 0.0f : velocity_.y);
	}
	const geom::Coord2 delta(velocity_*(geom::gFloat)(elapsedTime));
	position_ = Movable::move(collider_, position_, delta, map);
}

void Mover::setPosition(const geom::Coord2& position) {
	position_ = position;
}

const geom::Coord2& Mover::getPosition() const {
	return position_;
}

const geom::ShapeContainer& Mover::getCollider() const {
	return collider_;
}

void Mover::moveLeft()  { acceleration_.x = -ACCELERATION; }
void Mover::moveRight() { acceleration_.x =  ACCELERATION; }
void Mover::moveUp()    { acceleration_.y = -ACCELERATION; }
void Mover::moveDown()  { acceleration_.y =  ACCELERATION; }
void Mover::stopMovingHorizontal() { acceleration_.x = 0.0f; }
void Mover::stopMovingVertical()   { acceleration_.y = 0.0f; }
void Mover::stopMoving()           { acceleration_ = game::Acceleration2D(0, 0); }